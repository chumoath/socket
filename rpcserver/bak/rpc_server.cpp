#include <iostream>
#include "Calculator.h"


class CalculatorHandler : public Tutorial::CalculatorIf {
public:
    CalculatorHandler() {}

    int32_t add(const int32_t num1, const int32_t num2) {
        return num1 + num2;
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