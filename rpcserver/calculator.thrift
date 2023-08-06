namespace cpp Tutorial

struct CalDesc {
    1: i32 id;
    2: string op;
    3: i32 num1;
    4: i32 num2;
}

// thrift -r --gen cpp calculator.thrift
service Calculator {
    i32 ops(1: CalDesc desc)
}