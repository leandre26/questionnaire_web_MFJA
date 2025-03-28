#include <zmq.hpp>
#include <iostream>

int main() {
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5555");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0); // Subscribe to all messages

    std::cout << "Subscriber ready, waiting for message..." << std::endl;

    zmq::message_t message;
    subscriber.recv(message, zmq::recv_flags::none);
    std::string received(static_cast<char*>(message.data()), message.size());

    std::cout << "✅ Message reçu : " << received << std::endl;

    return 0;
}
