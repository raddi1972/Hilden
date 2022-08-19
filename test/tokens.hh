enum Token {
    KEY_HFLOAT,//keywords
    KEY_HCHAR,//keywords
    KEY_HINT,//keywords
    KEY_HSTRING, // keyword
    KEY_HWHILE, // keyword
    KEY_HPRINT, // keyword
    KEY_HIF, //keyword
    OP_PLUS,//operator
    OP_MINUS,//operator
    OP_MULT,//operator
    OP_DIV,//operator
    OP_EQUALS,//operator
    OP_NOT,//operator
    OP_LESS_THAN,//operator
    OP_GREATER_THAN,//operator
    OP_LESS_EQUAL, // operator
    OP_GREAT_EQUAL, // operator
    OP_DOUB_EQUAL, // operator
    OP_NOT_EQUAL, // operator
    SEMI_COL, // ends
    COMMA, // args
    OPEN_PAREN, // function
    CLOSE_PAREN, // function
    OPEN_CURLY, // block
    CLOSE_CURLY, // block
    LIT_HINT, // hint
    LIT_HFLOAT, // hfloat
    LIT_HSTRING, // hstring
    LIT_HCHAR, // hchar
    IDENTIFIER,
    END_OF,
};
