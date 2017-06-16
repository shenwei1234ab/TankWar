//
//  DBManager.cpp
//  demo_svr
//
//  Created by Wayne on 7/17/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "DBManager.h"

#include "vnMySQLConnection.h"
#include "vnMySQLStatement.h"
#include "vnLog.h"

#include <cstring>

struct DB_STMT {
    enum : size_t {
        kQueryPlayerId = 0,
        kQueryPlayerInfo,
        kInsertPlayer,
        kInsertAccount,
        kUpdatePlayerName,
        
        _count
    };
    
    const c8 *_sql;
    std::initializer_list<MySQL_DataType> _types;
} g_stmts[] = {
    {"select player_id from account where pp_id=?", {kMySQL_string}},
    {"select name from player where id=?", {kMySQL_u32}},
    {"insert into player values()", {}},
    {"insert into account values(?,?)", {kMySQL_string, kMySQL_u32}},
    {"update player set name=? where id=?", {kMySQL_string, kMySQL_u32}},
};

struct DB_Block : AllocatedObject {
    MySQL_ConnectionPtr conn;
    MySQL_StatementPtr stmts[DB_STMT::_count];
    
    bool init() {
        auto _conn = vnnew MySQL_Connection();
        if (!_conn->connect(server.c_str(), user.c_str(), passwd.c_str(), "demo")) {
            VN_LOG_ERROR("db connect FALID");
            delete _conn;
            return false;
        }
        conn.reset(_conn);
        for (size_t i = 0; i < (size_t)DB_STMT::_count; ++i) {
            stmts[i].reset(_conn->createStatement(g_stmts[i]._sql, strlen(g_stmts[i]._sql), g_stmts[i]._types));
            if (!stmts[i]) {
                return false;
            }
        }
        return true;
    }
    
    bool check() {
        if (conn->ping()) {
            return true;
        }
        VN_LOG_INFO("db connection lost.");
        if (!this->init()) {
            return false;
        }
        return true;
    }
    
    str8 server, user, passwd;
};

bool DBManager::startup(size_t threads, const c8 *server, const c8 *user, const c8 *passwd) {
    if (!threads) {
        threads = 1;
    }
    MySQL_Connection::initialize();
    
    auto init_fn = [server, user, passwd](void *&cb) -> bool {
        MySQL_Connection::thread_init();
        DB_Block *block = vnnew DB_Block;
        cb = block;
		block->server = server;
		block->user = user;
		block->passwd = passwd;
        return block->init();
    };
    auto fini_fn = [](void *cb) {
        if (cb) {
            delete static_cast<DB_Block *>(cb);
        }
        MySQL_Connection::thread_end();
    };
    
    if (m_pool.startup(threads, init_fn, fini_fn) != threads) {
        m_pool.cleanup();
        return false;
    }
    
    return true;
}

void DBManager::cleanup() {
    m_pool.cleanup();
    MySQL_Connection::finalize();
}

void DBManager::getPlayerId(str8 ppId, AsyncObjectPtr obj, std::function<void(bool _new, u32 playerId)> fn) {
    auto dbfn = [](void *cb, str8 ppId, AsyncObjectPtr obj, std::function<void(bool _new, u32 playerId)> fn) {
        auto block = static_cast<DB_Block *>(cb);
        if (!block->check()) {
            // TODO: error.
            return ;
        }
        u32 playerId = 0;
        auto stmt = block->stmts[DB_STMT::kQueryPlayerId].get();
        stmt->param(0)->set_external_string(ppId.c_str(), ppId.length());
        
        if (!stmt->execute()) {
            // TODO: error.
            return ;
        }
		//如果找到PPid就扔给obj所在的线程去执行回调函数
        if (stmt->fetch()) {
            playerId = stmt->field(0)->get_u32();
            stmt->free_result();
            auto p = obj.release();
            p->async(p, std::bind(std::move(fn), false, playerId));
            return;
        }
		//释放结果集
        stmt->free_result();
        

        stmt = block->stmts[DB_STMT::kInsertPlayer].get();
        
        if (!stmt->execute()) {
            // TODO: error.
            return ;
        }

        playerId = (u32)stmt->insert_id();
        
        stmt = block->stmts[DB_STMT::kInsertAccount].get();
        stmt->param(0)->set_external_string(ppId.c_str(), ppId.length());
        stmt->param(1)->set(playerId);
        if (!stmt->execute()) {
            // TODO: error.
            return ;
        }
        auto p = obj.release();
        p->async(p, std::bind(std::move(fn), true, playerId));
    };
    m_pool.post(std::bind(dbfn, std::placeholders::_1, std::move(ppId), std::move(obj), std::move(fn)));
}

void DBManager::loadPlayerInfo(u32 playerId, AsyncObjectPtr obj, std::function<void(bool result, PlayerInfo &info)> fn) {
    auto dbfn = [playerId](void *cb, AsyncObjectPtr obj, std::function<void(bool result, PlayerInfo &info)> fn) {
        auto block = static_cast<DB_Block *>(cb);
        PlayerInfo info;
        if (!block->check()) {
            obj->async(std::bind(std::move(fn), false, std::move(info)));
            return ;
        }
        auto stmt = block->stmts[DB_STMT::kQueryPlayerInfo].get();
        
        stmt->param(0)->set(playerId);
        if (!stmt->execute()) {
            obj->async(std::bind(std::move(fn), false, std::move(info)));
            return ;
        } else if (!stmt->fetch()) {
            stmt->free_result();
            auto p = obj.release();
            p->async(p, std::bind(std::move(fn), false, std::move(info)));
        }
        stmt->free_result();
        info.playerName = stmt->field(0)->get_string();
        auto p = obj.release();
        p->async(p, std::bind(std::move(fn), true, std::move(info)));
    };
    
    m_pool.post(std::bind(dbfn, std::placeholders::_1, std::move(obj), std::move(fn)));
}

void DBManager::setPlayerName(u32 playerId, str8 name, AsyncObjectPtr obj, std::function<void(bool result, str8 &name)> fn) {
    auto dbfn = [playerId](void *cb, str8 name, AsyncObjectPtr obj, std::function<void(bool result, str8 &name)> fn) {
        auto block = static_cast<DB_Block *>(cb);
        auto stmt = block->stmts[DB_STMT::kUpdatePlayerName].get();
        
        stmt->param(0)->set_external_string(name.c_str(), name.length());
        stmt->param(1)->set(playerId);
        auto p = obj.release();
        p->async(p, std::bind(std::move(fn), stmt->execute(false) && stmt->affected_rows(), std::move(name)));
    };
    
    m_pool.post(std::bind(dbfn, std::placeholders::_1, std::move(name), std::move(obj), std::move(fn)));
}
