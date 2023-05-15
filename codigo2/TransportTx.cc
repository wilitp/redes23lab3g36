#ifndef TX
#define TX

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class TransportTx: public cSimpleModule {
private:
    cQueue buffer;
    cMessage *endServiceEvent;
    cMessage *extraSpaceEvent;
    simtime_t serviceTime;
    cOutVector bufferSizeVector;
    cOutVector packetDropVector;
    cOutVector out_vector;
    long long receiver_buff_cap;
public:
    TransportTx();
    virtual ~TransportTx();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(TransportTx);

TransportTx::TransportTx() {
    endServiceEvent = NULL;
    extraSpaceEvent = NULL;
}

TransportTx::~TransportTx() {
    cancelAndDelete(endServiceEvent);
    cancelAndDelete(extraSpaceEvent);
}

void TransportTx::initialize() {
    buffer.setName("buffer");
    bufferSizeVector.setName("Buffer Size");
    packetDropVector.setName("Packet Drop");
    out_vector.setName("Out");
    endServiceEvent = new cMessage("endService");
    extraSpaceEvent = new cMessage("extraSpaceEvent");
    receiver_buff_cap = 10;
}

void TransportTx::finish() {
}

void TransportTx::handleMessage(cMessage *msg) {

    if(strcmp("extraSpaceEvent", msg->getName()) == 0){ //receiver has space in its buffer
        receiver_buff_cap++;
        //this->bubble("extra cap event arrived");
        if (!endServiceEvent->isScheduled()) {
            // start the service
            scheduleAt(simTime(), endServiceEvent);
        }
        return;
    }

    // if msg is signaling an endServiceEvent
    if (msg == endServiceEvent) {
        // if packet in buffer, send next one
        if (!buffer.isEmpty() && receiver_buff_cap > 0) {
            // dequeue packet
            cPacket *pkt = (cPacket*) buffer.pop();
            // send packet
            send(pkt, "toOut$o");
            receiver_buff_cap--;
            // start new service
            serviceTime = pkt->getDuration();
            scheduleAt(simTime() + serviceTime, endServiceEvent);
        }
    } else { // if msg is a data packet
        // check buffer limit
        if (buffer.getLength() >= par("bufferSize").intValue()){
            // drop the packet
            delete msg;
            this->bubble("packet dropped");
            packetDropVector.record(1);
        }
        else{
            // enqueue the packet
            buffer.insert(msg);
            bufferSizeVector.record(buffer.getLength());
            // if the server is idle
            if (!endServiceEvent->isScheduled()) {
                // start the service
                scheduleAt(simTime(), endServiceEvent);
            }
        }
    }
}

#endif /* QUEUE */
