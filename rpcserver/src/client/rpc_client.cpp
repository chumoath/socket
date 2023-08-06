#include <iostream>
#include "Calculator.h"

int main() {
    std::shared_ptr<apache::thrift::transport::TTransport> socket(new apache::thrift::transport::TSocket("192.168.0.162", 9090));
    std::shared_ptr<apache::thrift::transport::TTransport> transport(new apache::thrift::transport::TBufferedTransport(socket));
    std::shared_ptr<apache::thrift::protocol::TProtocol> protocol(new apache::thrift::protocol::TBinaryProtocol(transport));

    Tutorial::CalculatorClient client(protocol);

    try {
        transport->open();

        Tutorial::CalDesc desc;
        desc.num1 = 1;
        desc.num2 = 2;
        desc.op = "+";
        int32_t result = client.ops(desc);
        std::cout << desc.num1 << " + " << desc.num2 << " = " << result << std::endl;

        desc.op = "*";
        result = client.ops(desc);
        std::cout << desc.num1 << " * " << desc.num2 << " = " << result << std::endl;

        transport->close();
    } catch (apache::thrift::TException &tx) {
        std::cerr << "Error: " << tx.what() << std::endl;
    }

    return 0;
}