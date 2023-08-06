#include <iostream>
#include "Calculator.h"

class CalculatorHandler : public Tutorial::CalculatorIf {
public:
    CalculatorHandler() {}

    int32_t ops(const Tutorial::CalDesc & desc) {
        if (desc.op == "+") {
            return desc.num1 + desc.num2;
        } else if (desc.op == "-") {
            return desc.num1 - desc.num2;
        } else if (desc.op == "*") {
            return desc.num1 * desc.num2;
        } else if (desc.op == "/") {
            return desc.num1 / desc.num2;
        } else {
            return -1;
        }
    }
};

int main() {
    std::shared_ptr<CalculatorHandler> handler(new CalculatorHandler());
    std::shared_ptr<apache::thrift::TProcessor> processor(new Tutorial::CalculatorProcessor(handler));
    std::shared_ptr<apache::thrift::server::TServerTransport> serverTransport(new apache::thrift::transport::TServerSocket(9090));
    std::shared_ptr<apache::thrift::transport::TTransportFactory> transportFactory(new apache::thrift::transport::TBufferedTransportFactory());
    std::shared_ptr<apache::thrift::protocol::TProtocolFactory> protocolFactory(new apache::thrift::protocol::TBinaryProtocolFactory());

    apache::thrift::server::TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
    return 0;
}