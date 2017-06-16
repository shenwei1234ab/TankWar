// automatic generated by vnlib.
enum {
	PROD__0 = 0, // -> script
	PROD_additive_expression_0 = 36, //additive_expression -> multiplicative_expression
	PROD_additive_expression_1 = 37, //additive_expression -> additive_expression '+' multiplicative_expression
	PROD_additive_expression_2 = 38, //additive_expression -> additive_expression '-' multiplicative_expression
	PROD_argument_list_0 = 24, //argument_list -> expression
	PROD_argument_list_1 = 25, //argument_list -> argument_list ',' expression
	PROD_assignment_operator_0 = 55, //assignment_operator -> '='
	PROD_assignment_operator_1 = 56, //assignment_operator -> '*='
	PROD_assignment_operator_2 = 57, //assignment_operator -> '/='
	PROD_assignment_operator_3 = 58, //assignment_operator -> '%='
	PROD_assignment_operator_4 = 59, //assignment_operator -> '+='
	PROD_assignment_operator_5 = 60, //assignment_operator -> '-='
	PROD_base_type_0 = 61, //base_type -> 'bool'
	PROD_base_type_1 = 62, //base_type -> 'int'
	PROD_base_type_2 = 63, //base_type -> 'uint'
	PROD_base_type_3 = 64, //base_type -> 'long'
	PROD_base_type_4 = 65, //base_type -> 'ulong'
	PROD_base_type_5 = 66, //base_type -> 'float'
	PROD_base_type_6 = 67, //base_type -> 'double'
	PROD_class_member_0 = 123, //class_member -> struct_member
	PROD_class_member_1 = 124, //class_member -> 'new' '(' ')' compound_statement
	PROD_class_member_2 = 125, //class_member -> 'new' '(' parameter_list ')' compound_statement
	PROD_class_member_list_0 = 126, //class_member_list -> class_member
	PROD_class_member_list_1 = 127, //class_member_list -> class_member_list class_member
	PROD_class_specifier_0 = 115, //class_specifier -> 'class' @identifier '{' '}'
	PROD_class_specifier_1 = 116, //class_specifier -> 'class' @identifier '{' class_member_list '}'
	PROD_class_specifier_2 = 117, //class_specifier -> 'class' @identifier ':' '<' '>' '{' '}'
	PROD_class_specifier_3 = 118, //class_specifier -> 'class' @identifier ':' '<' user_type_list '>' '{' class_member_list '}'
	PROD_class_specifier_4 = 119, //class_specifier -> 'class' @identifier ':' user_type '{' '}'
	PROD_class_specifier_5 = 120, //class_specifier -> 'class' @identifier ':' user_type '{' class_member_list '}'
	PROD_class_specifier_6 = 121, //class_specifier -> 'class' @identifier ':' user_type '<' '>' '{' '}'
	PROD_class_specifier_7 = 122, //class_specifier -> 'class' @identifier ':' user_type '<' user_type_list '>' '{' class_member_list '}'
	PROD_compound_statement_0 = 168, //compound_statement -> '{' '}'
	PROD_compound_statement_1 = 169, //compound_statement -> '{' statement_list '}'
	PROD_conditional_expression_0 = 51, //conditional_expression -> logical_or_expression
	PROD_conditional_expression_1 = 52, //conditional_expression -> logical_or_expression '?' expression ':' conditional_expression
	PROD_constant_0 = 1, //constant -> 'nil'
	PROD_constant_1 = 2, //constant -> 'true'
	PROD_constant_2 = 3, //constant -> 'false'
	PROD_constant_3 = 4, //constant -> @integer
	PROD_constant_4 = 5, //constant -> @long_integer
	PROD_constant_5 = 6, //constant -> @float
	PROD_constant_6 = 7, //constant -> @double
	PROD_constant_7 = 8, //constant -> @string_literal
	PROD_container_type_0 = 68, //container_type -> 'array' '<' type_name ',' @integer '>'
	PROD_container_type_1 = 69, //container_type -> 'vector' '<' type_name '>'
	PROD_container_type_2 = 70, //container_type -> 'list' '<' type_name '>'
	PROD_container_type_3 = 71, //container_type -> 'map' '<' type_name ',' type_name '>'
	PROD_declaration_0 = 181, //declaration -> ';'
	PROD_declaration_1 = 182, //declaration -> namespace
	PROD_declaration_2 = 183, //declaration -> type_specifier
	PROD_declaration_3 = 184, //declaration -> function_header compound_statement
	PROD_declaration_list_0 = 185, //declaration_list -> declaration
	PROD_declaration_list_1 = 186, //declaration_list -> declaration_list declaration
	PROD_enum_specifier_0 = 88, //enum_specifier -> 'enum' '{' enumerator_list '}'
	PROD_enum_specifier_1 = 89, //enum_specifier -> 'enum' '{' enumerator_list ',' '}'
	PROD_enum_specifier_2 = 90, //enum_specifier -> 'enum' @identifier '{' enumerator_list '}'
	PROD_enum_specifier_3 = 91, //enum_specifier -> 'enum' @identifier '{' enumerator_list ',' '}'
	PROD_enumerator_0 = 94, //enumerator -> @identifier
	PROD_enumerator_1 = 95, //enumerator -> @identifier '=' conditional_expression
	PROD_enumerator_list_0 = 92, //enumerator_list -> enumerator
	PROD_enumerator_list_1 = 93, //enumerator_list -> enumerator_list ',' enumerator
	PROD_equality_expression_0 = 44, //equality_expression -> relational_expression
	PROD_equality_expression_1 = 45, //equality_expression -> equality_expression '==' relational_expression
	PROD_equality_expression_2 = 46, //equality_expression -> equality_expression '!=' relational_expression
	PROD_expression_0 = 53, //expression -> conditional_expression
	PROD_expression_1 = 54, //expression -> unary_expression assignment_operator expression
	PROD_expression_statement_0 = 155, //expression_statement -> ';'
	PROD_expression_statement_1 = 156, //expression_statement -> expression ';'
	PROD_function_header_0 = 137, //function_header -> 'function' @identifier '(' ')'
	PROD_function_header_1 = 138, //function_header -> 'function' @identifier '(' ')' ':' return_type
	PROD_function_header_2 = 139, //function_header -> 'function' @identifier '(' parameter_list ')'
	PROD_function_header_3 = 140, //function_header -> 'function' @identifier '(' parameter_list ')' ':' return_type
	PROD_function_type_0 = 72, //function_type -> 'function' '<' '(' ')' '>'
	PROD_function_type_1 = 73, //function_type -> 'function' '<' '(' parameter_list ')' '>'
	PROD_function_type_2 = 74, //function_type -> 'function' '<' '(' ')' ':' return_type '>'
	PROD_function_type_3 = 75, //function_type -> 'function' '<' '(' parameter_list ')' ':' return_type '>'
	PROD_initializer_0 = 132, //initializer -> expression
	PROD_initializer_1 = 133, //initializer -> '{' initializer_list '}'
	PROD_initializer_2 = 134, //initializer -> '{' initializer_list ',' '}'
	PROD_initializer_list_0 = 135, //initializer_list -> initializer
	PROD_initializer_list_1 = 136, //initializer_list -> initializer_list ',' initializer
	PROD_interface_member_0 = 110, //interface_member -> ';'
	PROD_interface_member_1 = 111, //interface_member -> type_specifier
	PROD_interface_member_2 = 112, //interface_member -> function_header
	PROD_interface_member_list_0 = 113, //interface_member_list -> interface_member
	PROD_interface_member_list_1 = 114, //interface_member_list -> interface_member_list interface_member
	PROD_interface_specifier_0 = 108, //interface_specifier -> 'interface' @identifier '{' '}'
	PROD_interface_specifier_1 = 109, //interface_specifier -> 'interface' @identifier '{' interface_member_list '}'
	PROD_iteration_statement_0 = 159, //iteration_statement -> 'while' '(' expression ')' statement
	PROD_iteration_statement_1 = 160, //iteration_statement -> 'do' statement 'while' '(' expression ')' ';'
	PROD_iteration_statement_2 = 161, //iteration_statement -> 'for' '(' expression_statement expression_statement ')' statement
	PROD_iteration_statement_3 = 162, //iteration_statement -> 'for' '(' expression_statement expression_statement expression ')' statement
	PROD_iteration_statement_4 = 163, //iteration_statement -> 'for' '(' variable_statement expression_statement ')' statement
	PROD_iteration_statement_5 = 164, //iteration_statement -> 'for' '(' variable_statement expression_statement expression ')' statement
	PROD_jump_statement_0 = 165, //jump_statement -> 'continue' ';'
	PROD_jump_statement_1 = 166, //jump_statement -> 'break' ';'
	PROD_jump_statement_2 = 167, //jump_statement -> 'return' expression_statement
	PROD_logical_and_expression_0 = 47, //logical_and_expression -> equality_expression
	PROD_logical_and_expression_1 = 48, //logical_and_expression -> logical_and_expression '&' equality_expression
	PROD_logical_or_expression_0 = 49, //logical_or_expression -> logical_and_expression
	PROD_logical_or_expression_1 = 50, //logical_or_expression -> logical_or_expression '|' logical_and_expression
	PROD_multiplicative_expression_0 = 32, //multiplicative_expression -> unary_expression
	PROD_multiplicative_expression_1 = 33, //multiplicative_expression -> multiplicative_expression '*' unary_expression
	PROD_multiplicative_expression_2 = 34, //multiplicative_expression -> multiplicative_expression '/' unary_expression
	PROD_multiplicative_expression_3 = 35, //multiplicative_expression -> multiplicative_expression '%' unary_expression
	PROD_namespace_0 = 179, //namespace -> 'namespace' @identifier '{' '}'
	PROD_namespace_1 = 180, //namespace -> 'namespace' @identifier '{' declaration_list '}'
	PROD_parameter_0 = 143, //parameter -> type_name
	PROD_parameter_1 = 144, //parameter -> 'ref' type_name
	PROD_parameter_2 = 145, //parameter -> @identifier ':' type_name
	PROD_parameter_3 = 146, //parameter -> 'ref' @identifier ':' type_name
	PROD_parameter_list_0 = 147, //parameter_list -> parameter
	PROD_parameter_list_1 = 148, //parameter_list -> parameter_list ',' parameter
	PROD_postfix_expression_0 = 15, //postfix_expression -> primary_expression
	PROD_postfix_expression_1 = 16, //postfix_expression -> postfix_expression '[' expression ']'
	PROD_postfix_expression_2 = 17, //postfix_expression -> postfix_expression '(' ')'
	PROD_postfix_expression_3 = 18, //postfix_expression -> postfix_expression '(' argument_list ')'
	PROD_postfix_expression_4 = 19, //postfix_expression -> postfix_expression '.' @identifier
	PROD_postfix_expression_5 = 20, //postfix_expression -> postfix_expression '::' @identifier
	PROD_postfix_expression_6 = 21, //postfix_expression -> postfix_expression '::' 'new'
	PROD_postfix_expression_7 = 22, //postfix_expression -> postfix_expression '++'
	PROD_postfix_expression_8 = 23, //postfix_expression -> postfix_expression '--'
	PROD_primary_expression_0 = 9, //primary_expression -> @identifier
	PROD_primary_expression_1 = 10, //primary_expression -> 'self'
	PROD_primary_expression_2 = 11, //primary_expression -> 'super'
	PROD_primary_expression_3 = 12, //primary_expression -> constant
	PROD_primary_expression_4 = 13, //primary_expression -> '(' expression ')'
	PROD_primary_expression_5 = 14, //primary_expression -> 'cast' '<' type_name '>' '(' expression ')'
	PROD_relational_expression_0 = 39, //relational_expression -> additive_expression
	PROD_relational_expression_1 = 40, //relational_expression -> relational_expression '<' additive_expression
	PROD_relational_expression_2 = 41, //relational_expression -> relational_expression '>' additive_expression
	PROD_relational_expression_3 = 42, //relational_expression -> relational_expression '<=' additive_expression
	PROD_relational_expression_4 = 43, //relational_expression -> relational_expression '>=' additive_expression
	PROD_return_type_0 = 141, //return_type -> type_name
	PROD_return_type_1 = 142, //return_type -> 'ref' type_name
	PROD_script_0 = 187, //script -> declaration_list
	PROD_selection_statement_0 = 157, //selection_statement -> 'if' '(' expression ')' statement
	PROD_selection_statement_1 = 158, //selection_statement -> 'if' '(' expression ')' statement 'else' statement
	PROD_statement_0 = 170, //statement -> variable_statement
	PROD_statement_1 = 171, //statement -> 'static' variable_statement
	PROD_statement_2 = 172, //statement -> expression_statement
	PROD_statement_3 = 173, //statement -> selection_statement
	PROD_statement_4 = 174, //statement -> iteration_statement
	PROD_statement_5 = 175, //statement -> jump_statement
	PROD_statement_6 = 176, //statement -> compound_statement
	PROD_statement_list_0 = 177, //statement_list -> statement
	PROD_statement_list_1 = 178, //statement_list -> statement_list statement
	PROD_struct_member_0 = 100, //struct_member -> ';'
	PROD_struct_member_1 = 101, //struct_member -> type_specifier
	PROD_struct_member_2 = 102, //struct_member -> variable_statement
	PROD_struct_member_3 = 103, //struct_member -> 'static' variable_statement
	PROD_struct_member_4 = 104, //struct_member -> function_header compound_statement
	PROD_struct_member_5 = 105, //struct_member -> 'static' function_header compound_statement
	PROD_struct_member_list_0 = 106, //struct_member_list -> struct_member
	PROD_struct_member_list_1 = 107, //struct_member_list -> struct_member_list struct_member
	PROD_struct_specifier_0 = 96, //struct_specifier -> 'struct' @identifier '{' '}'
	PROD_struct_specifier_1 = 97, //struct_specifier -> 'struct' @identifier '{' struct_member_list '}'
	PROD_struct_specifier_2 = 98, //struct_specifier -> 'struct' @identifier ':' user_type '{' '}'
	PROD_struct_specifier_3 = 99, //struct_specifier -> 'struct' @identifier ':' user_type '{' struct_member_list '}'
	PROD_type_name_0 = 80, //type_name -> base_type
	PROD_type_name_1 = 81, //type_name -> container_type
	PROD_type_name_2 = 82, //type_name -> container_type '::' @identifier
	PROD_type_name_3 = 83, //type_name -> function_type
	PROD_type_name_4 = 84, //type_name -> user_type
	PROD_type_name_5 = 85, //type_name -> 'ptr' '<' user_type '>'
	PROD_type_name_list_0 = 86, //type_name_list -> type_name
	PROD_type_name_list_1 = 87, //type_name_list -> type_name_list ',' type_name
	PROD_type_specifier_0 = 128, //type_specifier -> enum_specifier
	PROD_type_specifier_1 = 129, //type_specifier -> struct_specifier
	PROD_type_specifier_2 = 130, //type_specifier -> interface_specifier
	PROD_type_specifier_3 = 131, //type_specifier -> class_specifier
	PROD_unary_expression_0 = 26, //unary_expression -> postfix_expression
	PROD_unary_expression_1 = 27, //unary_expression -> '++' unary_expression
	PROD_unary_expression_2 = 28, //unary_expression -> '--' unary_expression
	PROD_unary_expression_3 = 29, //unary_expression -> '+' unary_expression
	PROD_unary_expression_4 = 30, //unary_expression -> '-' unary_expression
	PROD_unary_expression_5 = 31, //unary_expression -> '!' unary_expression
	PROD_user_type_0 = 76, //user_type -> @identifier
	PROD_user_type_1 = 77, //user_type -> user_type '::' @identifier
	PROD_user_type_list_0 = 78, //user_type_list -> user_type
	PROD_user_type_list_1 = 79, //user_type_list -> user_type_list ',' user_type
	PROD_variable_0 = 150, //variable -> @identifier ':' type_name
	PROD_variable_1 = 151, //variable -> @identifier '=' initializer
	PROD_variable_2 = 152, //variable -> @identifier ':' type_name '=' initializer
	PROD_variable_list_0 = 153, //variable_list -> variable
	PROD_variable_list_1 = 154, //variable_list -> variable_list ',' variable
	PROD_variable_statement_0 = 149, //variable_statement -> 'var' variable_list
	PROD_COUNT = 188
};
