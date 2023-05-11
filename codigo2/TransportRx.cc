#ifndef RX
#define RX

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class TransportRx: public cSimpleModule {
private:
    cQueue buffer;
    cMessage *endServiceEvent;
    cMessage *extraSpaceEvent;
    simtime_t serviceTime;
    cOutVector bufferSizeVector;
    cOutVector packetDropVector;
    cOutVector out_vector;
public:
    TransportRx();
    virtual ~TransportRx();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(TransportRx);

TransportRx::TransportRx() {
    endServiceEvent = NULL;
    extraSpaceEvent = NULL;
}

TransportRx::~TransportRx() {
    cancelAndDelete(endServiceEvent);
    cancelAndDelete(extraSpaceEvent);
}

void TransportRx::initialize() {
    buffer.setName("buffer");
    endServiceEvent = new cMessage("endService");
    //send(extraSpaceEvent, "toOut$o");
    
}

void TransportRx::finish() {
}

void TransportRx::handleMessage(cMessage *msg) {

    // if msg is signaling an endServiceEvent
    if (msg == endServiceEvent) {
        // if packet in buffer, send next one
        if (!buffer.isEmpty()) {
            // dequeue packet
            cPacket *pkt = (cPacket*) buffer.pop();
            // send packet
            send(pkt, "toApp");
            //tell the sender we have space in the buffer
            extraSpaceEvent = new cMessage("extraSpaceEvent");
            send(extraSpaceEvent, "toOut$o");
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
