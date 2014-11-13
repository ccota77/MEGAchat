
/* This code is based on strophe.jingle.js by ESTOS */
#include <string>
#include <map>
#include <memory>
#include "webrtcAdapter.h"
#include "strophe.jingle.session.h"
#include <mstrophepp.h>

namespace disco
{
    class DiscoPlugin;
}


namespace karere
{
namespace rtcModule
{
class JingleSession;
//TODO: Implement
class FileTransferHandler;
struct AnswerOptions
{
    artc::tspMediaStream localStream;
    AvFlags av;
};
typedef std::function<bool(bool, std::shared_ptr<AnswerOptions>, const char* reason,
   const char* text)> CallAnswerFunc;

class ICryptoFunctions
{
public:
    class IString
    {
    public:
        virtual ~IString(){}
        virtual const char* c_str() const = 0;
        virtual bool empty() const = 0;
    };

    virtual IString* generateMac(const char* data, const char* key) = 0;
    virtual IString* decryptMessage(const char* msg) = 0;
    virtual IString* encryptMessageForJid(const char* msg, const char* jid) = 0;
    virtual void preloadCryptoForJid(const char* jid, void* userp,
        void(*cb)(void* userp, const char* errMsg)) = 0;
    virtual IString* scrambleJid(const char* jid) = 0;
    virtual IString* generateFprMacKey() = 0;
    virtual ~ICryptoFunctions() {}
};
//Convenience type to accomodate returned IString objects from ICryptoFunctions api
struct VString: public std::unique_ptr<ICryptoFunctions::IString>
{
    typedef std::unique_ptr<ICryptoFunctions::IString> Base;
    VString(ICryptoFunctions::IString* obj): Base(obj){}
    bool empty() {return get()->empty();}
    const char* c_str() const {return get()->c_str();}
};

class Jingle: strophe::Plugin
{
protected:
/** Contains all info about a not-yet-established session, when onCallTerminated is fired and there is no session yet */
    struct FakeSessionInfo: public IJingleSession
    {
        const std::string mSid;
        const std::string mPeer;
        const std::string mJid;
        bool mIsInitiator;
        std::string mPeerAnonId;
        FakeSessionInfo(const std::string& aSid, const std::string& aPeer,
            const std::string& aMyJid, bool aInitiator, const std::string& peerAnonId)
            :mSid(aSid), mPeer(aPeer), mJid(aMyJid), mIsInitiator(aInitiator){}
        virtual bool isRealSession() const {return false;}
        virtual const char* getSid() const {return mSid.c_str();}
        virtual const char* getJid() const {return mJid.c_str();}
        virtual const char* getPeerJid() const {return mPeer.c_str();}
        virtual const char* getPeerAnonId() const {return mPeerAnonId.c_str();}
        virtual bool isCaller() const {return mIsInitiator;}
        virtual int isRelayed() const {return false;}
        virtual void setUserData(void*, DeleteFunc delFunc) {}
        virtual void* getUserData() const {return nullptr;}
    };
/** Contains all info about an incoming call that has been accepted at the message level and needs to be autoaccepted at the jingle level */
    struct AutoAcceptCallInfo: public StringMap
    {
        Ts tsReceived;
        Ts tsTillJingle;
        std::shared_ptr<AnswerOptions> options;
        std::shared_ptr<FileTransferHandler> ftHandler;
    };
    typedef std::map<std::string, std::shared_ptr<AutoAcceptCallInfo> > AutoAcceptMap;
    std::map<std::string, JingleSession*> mSessions;
/** Timeout after which if an iq response is not received, an error is generated */
    int mJingleTimeout = 50000;
/** The period, during which an accepted call request will be valid
* and the actual call answered. The period starts at the time the
* request is received from the network */
    int mJingleAutoAcceptTimeout = 15000;
/** The period within which an outgoing call can be answered by peer */
    int callAnswerTimeout = 50000;
    AutoAcceptMap mAutoAcceptCalls;
    std::unique_ptr<ICryptoFunctions> mCrypto;
    std::string mOwnFprMacKey;
    std::string mOwnAnonId;
public:
    enum {DISABLE_MIC = 1, DISABLE_CAM = 2, HAS_MIC = 4, HAS_CAM = 8};
    int mediaFlags = 0;

    std::shared_ptr<webrtc::PeerConnectionInterface::IceServers> mIceServers;
    webrtc::FakeConstraints mMediaConstraints;
    artc::DeviceManager deviceManager;
    ICryptoFunctions& crypto() const {return *mCrypto;}
//event handler interface
    virtual void onConnectionEvent(int state, const std::string& msg){}
    virtual void onRemoteStreamAdded(JingleSession& sess, artc::tspMediaStream stream){}
    virtual void onRemoteStreamRemoved(JingleSession& sess, artc::tspMediaStream stream){}
    virtual void onJingleError(JingleSession* sess, const std::string& origin,
        const std::string& stanza, strophe::Stanza orig, char type='s'){} //TODO: implement stanza object in promise errors
    virtual void onJingleTimeout(JingleSession& sess, const std::string& err, strophe::Stanza orig){}
//    virtual void onIceConnStateChange(JingleSession& sess, event){}
    virtual void onIceComplete(JingleSession& sess){}
//rtcHandler callback interface, called by the connection.jingle object
    virtual void onIncomingCallRequest(const char* from, std::shared_ptr<CallAnswerFunc>& ans,
        std::shared_ptr<std::function<bool()> >& reqStillValid,
        const AvFlags& peerMedia, std::shared_ptr<std::set<std::string> >& files)
    {}
    virtual void onCallCanceled(const char* peer, const char* event,
     const char* by, bool accepted){}
    //virtual void onCallAnswerTimeout(const char* peer) {} Generated by the higher level
    virtual void onCallAnswered(JingleSession& sess) {}
    virtual void onCallTerminated(JingleSession* sess, const char* reason,
      const char* text, const FakeSessionInfo* info=NULL){}
    virtual bool onCallIncoming(JingleSession& sess, std::string& reason,
                                std::string& text){return true;}
    virtual void onRinging(JingleSession& sess){}
    virtual void onMuted(JingleSession& sess, const AvFlags& affected){}
    virtual void onUnmuted(JingleSession& sess, const AvFlags& affected){}
    virtual void onInternalError(const std::string& msg, const char* where);
//==
    Jingle(strophe::Connection& conn, ICryptoFunctions* crypto, const std::string& iceServers="");
    void addAudioCaps(disco::DiscoPlugin& disco);
    void addVideoCaps(disco::DiscoPlugin& disco);
    void registerDiscoCaps();
    void setIceServers(const std::string& servers){}
    void onConnState(const xmpp_conn_event_t status,
        const int error, xmpp_stream_error_t * const stream_error);
/*    int _static_onJingle(xmpp_conn_t* const conn,
        xmpp_stanza_t* stanza, void* userdata);
    static int _static_onIncomingCallMsg(xmpp_conn_t* const conn,
        xmpp_stanza_t* stanza, void* userdata);
*/
    void onJingle(strophe::Stanza iq);
    /* Incoming call request with a message stanza of type 'megaCall' */
    void onIncomingCallMsg(strophe::Stanza callmsg);
    bool cancelAutoAcceptEntry(const char* sid, const char* reason,
        const char* text, char type=0);
    bool cancelAutoAcceptEntry(AutoAcceptMap::iterator it, const char* reason,
    const char* text, char type=0);
    void cancelAllAutoAcceptEntries(const char* reason, const char* text);
    void purgeOldAcceptCalls();
    void processAndDeleteInputQueue(JingleSession& sess);
    promise::Promise<std::shared_ptr<JingleSession> >
      initiate(const char* sid, const char* peerjid, const char* myjid,
        artc::tspMediaStream sessStream, const AvFlags& avState,
        std::shared_ptr<StringMap> sessProps, FileTransferHandler* ftHandler=NULL);
    JingleSession* createSession(const char* me, const char* peerjid,
        const char* sid, artc::tspMediaStream, const AvFlags& avState,
        const StringMap& sessProps, FileTransferHandler* ftHandler=NULL);
    void terminateAll(const char* reason, const char* text, bool nosend=false);
    bool terminateBySid(const char* sid, const char* reason, const char* text,
        bool nosend=false);
    bool terminate(JingleSession* sess, const char* reason, const char* text,
        bool nosend=false);
    promise::Promise<strophe::Stanza> sendTerminateNoSession(const char* sid,
        const char* to, const char* reason, const char* text);
    promise::Promise<strophe::Stanza> sendIq(strophe::Stanza iq, const std::string& origin);
    bool sessionIsValid(const karere::rtcModule::JingleSession &sess);
    std::string getFingerprintsFromJingle(strophe::Stanza j);
    bool verifyMac(const std::string& msg, const std::string& key, const std::string& actualMac);
};
}
}
