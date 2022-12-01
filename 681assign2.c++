#pragma GCC target ("avx2")
#pragma GCC optimization ("O3")
#pragma GCC optimization ("unroll-loops")

#include<bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

#include "/home/CLionProjects/untitled/CSES/boost_1_78_0/boost/math/distributions/pareto.hpp"

using namespace __gnu_pbds;
using namespace std;
#define w           int x125;cin>>x125;while(x125--)
#define F10            ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0)
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
#include "lcgrand.h"
#include "rand.h"

#define al(x)        (x).begin(), (x).end()
#define ff            first
#define ss            second
#define ll           long long float
#define pb            push_back
#define mp            make_pair
#define pii           pair<ll,ll>
#define vi            vector<ll>
#define mii           map<ll,ll>
#define pqb           priority_queue<ll>
#define pqs           priority_queue<ll,vi,greater<ll>>
#define setbits(x)    __builtin_popcountll(x)
#define zrobits(x)    __builtin_ctzll(x)
#define mod           100
#define inf           1e18
#define ps(x, y)       fixed<<setprecision(y)<<x
#define mk(arr, n, type) type *arr=new type[n];

// start code
// round robin

int seed;
enum DistributionType{EXPONENTIAL,NORMAL,CONSTANT};
DistributionType probid;

float uniformDist(float end) {
    default_random_engine generator(rand());
    uniform_real_distribution<double> distribution (0.0,end);
    return distribution(generator);
}
float expon(float mean,enum DistributionType probDistribution)  /* Exponential variate generation function. */
{
    /*
    probDistrbution:{1-exponential}{2-uniform}{3-constant}

    */
    if(probDistribution==EXPONENTIAL)
        return -mean * log(lcgrand(seed));
    else if(probDistribution==NORMAL)
    {
        return uniformDist(mean);
    }
    return mean;
}

class Request{
public:
    float WebSerTime,DbmsSerTime,Remafloatime,NxtEventTime,startTime,pid;
    /*
    WebSerTime: Service Time at web server
    DbmsSerTime: Service Time at DBMS server
    Remafloatime:Remaining
    pid: Request ID
    serverid:{0-User}{1-Web Server}{2-DBMS Server}
    */
    enum station{USER,WEB,DBMS};
    station serverid;
    void prfloat(){
        cout<<" DbmsSerTime "<<DbmsSerTime<<
            " WebSerTime "<<WebSerTime<<" NxtEventTime "<<NxtEventTime<<" Remafloatime "<<Remafloatime<<endl;
    }
};
struct CompareRequest {
    bool operator()(Request const& p1, Request const& p2)
    {
        // return "true" if "p1" is ordered
        // before "p2", for example:
        return p1.NxtEventTime > p2.NxtEventTime;
    }
};
vector<vector<float>> answer(11);
class Simulation{
public:
    float dropPacket=0,totalServiceTime=0,totalWaitTime=0,successPacket=0, webcoreUtilisation=0,webnoncoreUtilisation=0, dbmscoreUtilisation=0
    , webcore , dbmscore,timeout=0,webServiceTime=0,webWaitTime=0,dbmsServiceTime=0,dbmsWaitTime=0,endTime=0;
    DistributionType probDistribution;
    /*
    dropPacket: Number of dropped packets
    totalServiceTime: Total service time at Web Server and DBMS Server
    totalWaitTime: Total wait time at Web Server and DBMS Server
    successPacket: Number of Packets which are successfully reached to user without Timeout
    webcoreUtilisation: Utilisation of Web Server
    dbmscoreUtilisation: Utilisation of DBMS Server
    */
    default_random_engine generator;
    float numofusers;
    uniform_real_distribution<double> distribution ;
    Simulation(float webCore , float dbmsCore,float num ){//,DistributionType prob , default_random_engine &g, uniform_real_distribution<double> &d){

        webcore=webCore;
        dbmscore=dbmsCore;
        numofusers=num;
//        probDistribution=prob;
//        generator=g;
//        distribution=d;
    }

    float expon(float mean,enum DistributionType probDistribution)  /* Exponential variate generation function. */
    {
        /*
        probDistrbution:{1-exponential}{2-uniform}{3-constant}

        */
        if(probDistribution==EXPONENTIAL)
            return -mean * log(lcgrand(seed));
        else if(probDistribution==NORMAL)
        {
            return distribution(generator);
        }
        return mean;
    }

    priority_queue<Request , vector<Request> , CompareRequest> topreq;
    /*
    Priority Queue for checking ordering of requests
    */
    Request toppacket()
    {
        //Return request which can be departed first based on its next event time
        Request packet = topreq.top();
        topreq.pop();
        return packet;
    }
    void report(float webtime ,float dbmstime , float totalPacket)
    {
        cout<<"Drop Packet Rate:"<<dropPacket/totalPacket<<" Success Packet :"<<successPacket<<
            " Web Service Time :"<<webServiceTime<<
            " DBMS Service Time :"<<dbmsServiceTime
            <<" Total Service Time :"<<totalServiceTime<<" Total Wait Time :"<<totalWaitTime
            <<" webcoreUtilisation:"<<webcoreUtilisation/(webtime*webcore)<< endl<<
            " dbmscoreUtilisation:"<<dbmscoreUtilisation/(dbmstime*dbmscore)<<" dbms wait time :"<<dbmsWaitTime<<
            " Through put :" <<(successPacket+timeout)/endTime<<" Badput :"<<timeout/endTime<<" GoodPut :"<<successPacket/endTime<<" TimeOut Request:"<<timeout<<endl;
        answer[1].push_back(dropPacket);
        answer[2].push_back(successPacket/(successPacket+dropPacket+timeout));
        answer[3].push_back((successPacket+timeout)/(endTime*webcore));
        answer[4].push_back(timeout/endTime);
        answer[5].push_back(successPacket/endTime);
        answer[6].push_back(webcoreUtilisation/(webtime*webcore));
        answer[7].push_back(dbmscoreUtilisation/(dbmstime*dbmscore));
        answer[8].push_back((totalWaitTime+totalServiceTime)*webcore/(successPacket));
        answer[9].push_back((webServiceTime+webWaitTime)/webcore);
        answer[10].push_back((dbmsWaitTime+dbmsServiceTime)/dbmscore);
        answer[0].push_back(numofusers);
    }
};

class Server{
public:
    void arrive(Request, Simulation) {};
    Request depart(Simulation) {  };

};

class Users : public Server{
public:
    float timeout,thinktime;
    /*
    timeout: Request timeout
    thinktime: Think time
    */

    //Sending Requests from User to WebServer with below parameters assigned
    Users(float timeout1, float thinktime1 , int numberOfRequest1,
          float dbmsmean ,float webmean,float floaterarrivalmean , Simulation &s,enum DistributionType probDistribution)
    {
        timeout=timeout1;
        thinktime=thinktime1;
        Request packet;
        for (int i = 0; i < numberOfRequest1; ++i) {
            packet.pid=i;
            //Defining Distribution for Web server and DBMS Service Time based on User Input
            packet.DbmsSerTime = expon(dbmsmean,probDistribution);
            packet.WebSerTime = expon(webmean,probDistribution);
            //Requests are initially at User
            packet.serverid=packet.USER;
            //

            packet.startTime = packet.NxtEventTime = expon(floaterarrivalmean,probDistribution);// 100/1000 =
            //Remaining Service Time Initialization
            packet.Remafloatime=0;

            s.topreq.push(packet);
        }
//        for (int i = 0; i < numberOfRequest1; ++i) {
//            cout<<probDistribution<<" "<<s.topreq.top().NxtEventTime<<endl;
//            s.topreq.pop();
//        }

    }

    void arrive(Request packet , Simulation &s)
    {
        // If no timeout modify number of successful Requests
        s.endTime=max(s.endTime , packet.NxtEventTime);
        if(packet.NxtEventTime-packet.startTime<=timeout)
        {
            s.successPacket++;
        }
            //If request got timeout then again send request to Web Server after User Think Time
        else {
            s.timeout++;//badput
        }
        packet.NxtEventTime += thinktime;
        packet.startTime = packet.NxtEventTime;
        s.topreq.push(packet);

    }
    Request depart(Simulation &s)
    {
        Request curr = s.toppacket();
        curr.serverid=curr.USER;
        return curr;
    }


};

class WebServer: public Server{
public:
    float prevTime=0,numofcore,queuesize,timeUnit,probNxtServer,fixcore;
    bool isDepart;
    vector<Request> queue;
    /*
    fixcore:Number of Cores of Web Server
    queuesize:Queue Size of Web Server
    numofcore:Number of Cores which are Idle
    probNextServer:Probability that Request will go to DBMS Server
    */
    Request p,p1;
    WebServer(float core ,float queueSize , float prob , float rrtimeUnit)
    {
        fixcore=numofcore = core;
        queuesize =queueSize;
        probNxtServer=prob;
        timeUnit=rrtimeUnit;
    }

    void arrive(Request packet , Simulation &s){


        // Check if Request arrived from User
        if(packet.serverid==packet.USER)
        {
            packet.serverid=packet.WEB;
            //If queue at Web Server is Full drop the requests and return
            if(queue.size()==queuesize)
            {
                s.dropPacket++;
                return;
            }
            //If queue is not full then place we can process that request at web  server
            s.totalServiceTime+=packet.WebSerTime;
            s.webServiceTime+=packet.WebSerTime;

            //Check If all cores are Idle
            if(numofcore == 0){
                queue.push_back(packet);
                return;
            }
            //If cores are Idle assign core to process request
            numofcore--;

        }
        //Modify the status of request as it is present at Web Server
        packet.serverid=packet.WEB;

        //Round Robin Scheduling of requests arriving at web server
        float reqtime = min(timeUnit , packet.WebSerTime);
        packet.Remafloatime = packet.WebSerTime - reqtime;
        packet.NxtEventTime=prevTime+reqtime;
        s.topreq.push(packet);

    }

    Request depart(Simulation &s){

        // Assume if we need to  DBMS Server which depends on Probability of DBMS Server
        p.serverid=p.DBMS;
        p.NxtEventTime=prevTime;

        float val =rand()%100;
        // 70 user 30 dbms
        // val <= probNxtServer then move to User server
        //Based on probability move request to User
        if(val <= probNxtServer)
        {
            p.serverid = p.USER;

            numofcore++;
            // assign new Request to Web Server
            if(queue.size()>0)
            {
                //Update Idle Number of Cores
                numofcore--;
                p1 = *queue.begin();
                queue.erase(queue.begin());
                p1.NxtEventTime = prevTime;
                s.topreq.push(p1);
            }
        }
        return p;
    }

    void process(Simulation &s , Request packet)
    {
        isDepart= false;
        //Find waiting time of request in queue
        s.webWaitTime+=(queue.size()* (packet.NxtEventTime - prevTime));
        s.totalWaitTime+=(queue.size()* (packet.NxtEventTime - prevTime));
        //Web server utilization of busy cores
        s.webcoreUtilisation+=((fixcore-numofcore)* (packet.NxtEventTime - prevTime));
        prevTime=packet.NxtEventTime;
        //If request is completed schedule departure
        if(packet.Remafloatime==0)
        {
            p = packet;
            isDepart = true;
        }
        else{
            // swap a request
            queue.push_back(packet);
            p = *queue.begin();
            queue.erase(queue.begin());

            //Round Robin Scheduling of requests
            float reqtimeunit = min(timeUnit , p.Remafloatime);
            p.Remafloatime-=reqtimeunit;
            p.NxtEventTime=packet.NxtEventTime + reqtimeunit;
            s.topreq.push(p);
        }

    }

};

class DbmsServer: public Server{
public:
    float prevTime=0,numofcore,queuesize,timeUnit,fixedcore;
    vector<Request> queue;
    bool isdepart=false;
    Request p,p1;
    DbmsServer(float core ,float queueSize , float rrtimeunit)
    {
        fixedcore=numofcore = core;
        queuesize =queueSize;
        timeUnit = rrtimeunit;

    }
    void arrive(Request packet , Simulation &s){
        // Packet arrive from User or arrive from Dbms
        packet.serverid=packet.DBMS;
        // queue is either full or empty
        if(queue.size()==queuesize)
        {
            s.dropPacket++;
            return;
        }
        s.totalServiceTime+=packet.WebSerTime;
        s.dbmsServiceTime+=packet.WebSerTime;
        // core is busy or not

        if(numofcore == 0){
            queue.push_back(packet);
            return;
        }
        numofcore--;
        float reqtime = min(timeUnit , packet.Remafloatime);
        packet.Remafloatime = packet.WebSerTime - reqtime;
        packet.NxtEventTime=prevTime+reqtime;
        s.topreq.push(packet);

    }

    Request depart(Simulation &s){
        //Update number of Idle cores
        numofcore++;
        // Check if queue is not empty
        if(queue.size()>0)
        {
            //Update number of Busy cores
            numofcore--;
            p1 = *queue.begin();
            queue.erase(queue.begin());
            p1.NxtEventTime = prevTime;
            s.topreq.push(p1);
        }
        return p;
    }

    void process(Simulation &s , Request packet)
    {
        isdepart=false;
        // Adding the wait time which is dependent on queue size, Previous timestamp and current timestamp
        s.dbmsWaitTime+=(queue.size()* (packet.NxtEventTime - prevTime));
        s.totalWaitTime+=(queue.size()* (packet.NxtEventTime - prevTime));
        // Adding the wait time which is dependent on Busy Cores, Previous timestamp and current timestamp
        s.dbmscoreUtilisation+=((fixedcore-numofcore)* (packet.NxtEventTime - prevTime));
        prevTime=packet.NxtEventTime;
        //If Remaining Time is zero Then Depart the request
        if(packet.Remafloatime==0)
        {

            p = packet;
            isdepart = true;
        }
        else{
            // swap a request
            queue.push_back(packet);
            p = *queue.begin();
            queue.erase(queue.begin());
            //Round Robin Scheduling of Requests
            float reqtimeunit = min(timeUnit , p.Remafloatime);
            p.Remafloatime-=reqtimeunit;
            p.NxtEventTime=packet.NxtEventTime + reqtimeunit;
            s.topreq.push(p);
        }

    }



};


int32_t main() {
    F10;
//    std::default_random_engine generator (rand());
//
//    std::uniform_real_distribution<double> distribution (0.0,10);
//    cout<<expon(5,EXPONENTIAL)<<endl;
//    cout<<expon(5,EXPONENTIAL)<<endl;
//    return 0;

    float timeout  , thinktime,numOfUsers, numofreq ,dbmsmean, webmean ,rrtimeunit,
            floaterarrivalmean  , webCore ,webQSize  ,webProb , dbmsCore , dbmsQSize ,inpprobid;
    cin>>timeout >> thinktime>> numofreq >>numOfUsers>>dbmsmean >> webmean >>rrtimeunit>>
    floaterarrivalmean >> webCore >>webQSize >>webProb >> dbmsCore >> dbmsQSize;

    cin>>inpprobid;
    cout<<"\nTimeout                                  :"<<timeout
        <<"\nThink Time                               :"<<thinktime
        <<"\nNumber of Requests                       :"<<numofreq
        <<"\nNumber of User                           :"<<numOfUsers
        <<"\nDBMS Service Mean Time                   :"<<dbmsmean
        <<"\nWeb Server Service Mean Time             :"<<webmean
        <<"\nRound Robin Quantum Time                 :"<<rrtimeunit
        <<"\nInterarrival Mean Time                   :"<<floaterarrivalmean
        <<"\nWeb Cores                                :"<<webCore
        <<"\nWeb Queue Size                           :"<<webQSize
        <<"\nDeparture Probaility of web              :"<<webProb
        <<"\nDBMS Cores                               :"<<dbmsCore
        <<"\nDBMS Queue Size                          :"<<dbmsQSize
        <<"\nProbability Type                         :"<<inpprobid<<"\n";
    probid=(DistributionType)inpprobid;
    string s1(150,'-');
//    numOfUsers=0;
//    numofreq=0;
    w{

//        numOfUsers+=30;
//        numofreq+=30;
        cout<<s1<<endl;
        cout<<"simTime"<<"\t\t\t\tRequest ID"<<"\t\t\t\tEvent_type"<<
        "\t\t\t\tQueue Size"<<"\t\t\t\tServer Type"<<"\t\t\t\tNext station"<<endl;
        cout<<s1<<endl;
        seed=rand()%100;
        default_random_engine generator(rand());
        uniform_real_distribution<double> distribution(0.0,5);
        Simulation simObj(webCore , dbmsCore,numOfUsers);
        Users useObj(timeout , thinktime , numOfUsers , dbmsmean , webmean , floaterarrivalmean,simObj,probid);
        DbmsServer dbmsObj(dbmsCore , dbmsQSize,rrtimeunit);
        WebServer webObj(webCore , webQSize , webProb,rrtimeunit);
        // Simulation start form here...
        //Simulation till user sends requests or all pa
        int i=0;
        while ((!simObj.topreq.empty() && simObj.successPacket+simObj.timeout< numofreq))
        {
            // Request fom user
            //cout<<"Request ID:"<<simObj.topreq.top().pid<<"\tAt server:"<<simObj.topreq.top().serverid<<"\tSuccessful Packets:"<< simObj.successPacket<<"\tPacket Drops:"<<simObj.dropPacket<<"\tPacket Timeouts:"<<simObj.timeout<<endl;
//            cout<<simObj.topreq.top().pid<<simObj.topreq.top().serverid<< simObj.successPacket <<simObj.dropPacket<<simObj.timeout;

            //If request is from user send it to Web Server
            Request packet;

            switch (simObj.topreq.top().serverid) {

                case Request::USER:
                    packet = useObj.depart(simObj);
                    if(i==0)
                    {
                        i++;
                        webObj.prevTime=packet.NxtEventTime;
                    }
                    webObj.arrive(packet, simObj);
                    cout<<ps(packet.NxtEventTime,5)<<"\t\t\t\t\t"
                        <<packet.pid<<"\t\t\t\t"
                        <<"Departure\t\t\t\t"
                        <<webObj.queue.size()<<"\t\t\t\t"
                        <<"Client Station\t\t\t\t"
                        <<"Web Server\t\t\t\t"<<endl;
                    break;
                case Request::WEB :
                    packet = simObj.toppacket();
                    webObj.process(simObj, packet);
                    if (webObj.isDepart) {
                        packet = webObj.depart(simObj);
                        //If request processing is completed send it to User
                        if (packet.serverid == packet.USER) {
                            useObj.arrive(packet, simObj);
                            cout<<packet.NxtEventTime<<"\t\t\t\t"
                                <<packet.pid<<"\t\t\t\t"
                                <<"Departure\t\t\t\t"
                                <<"NA\t\t\t\t"
                                <<"Web Server\t\t\t\t"
                                <<"Client Station\t\t\t\t"<<endl;
                        }
                            //If request is for DBMS Server
                        else {
                            if(i==1)
                            {
                                i++;
                                dbmsObj.prevTime=packet.NxtEventTime;
                            }
                            dbmsObj.arrive(packet, simObj);
                            cout<<packet.NxtEventTime<<"\t\t\t\t"
                                <<packet.pid<<"\t\t\t\t"
                                <<"Departure\t\t\t\t"
                                <<dbmsObj.queue.size()<<"\t\t\t\t"
                                <<"Web Server\t\t\t\t"
                                <<"DBMS Server\t\t\t\t"<<endl;
                        }
                    }
                    else{
                        cout<<packet.NxtEventTime<<"\t\t\t\t"
                            <<packet.pid<<"\t\t\t\t"
                            <<"Scheduling\t\t\t\t"
                            <<webObj.queue.size()<<"\t\t\t\t"
                            <<"Web Server\t\t\t\t"
                            <<"Web Server\t\t\t\t"<<endl;
                    }
                    break;

                    //If request is at DBMS Server process the request
                case Request::DBMS:
                    packet = simObj.toppacket();

                    dbmsObj.process(simObj, packet);
                    //If request processing at DBMS Server is completed send it to Web Server
                    if (dbmsObj.isdepart) {
                        packet = dbmsObj.depart(simObj);
                        webObj.arrive(packet, simObj);
                        cout<<packet.NxtEventTime<<"\t\t\t\t"
                            <<packet.pid<<"\t\t\t\t"
                            <<"Arrival\t\t\t\t\t"
                            <<webObj.queue.size()<<"\t\t\t\t"
                            <<"DBMS Server\t\t\t\t"
                            <<"Web Server\t\t\t\t"<<endl;
                    }
                    break;
            }

        }

        //Report of Simulation
        cout<<"Experiment Number "<<x125<<" ";
        simObj.report(webObj.prevTime , dbmsObj.prevTime , numofreq);
//        cout<<numofreq;
    };
    int zz=12;
    for (int i = 1; i < 11; ++i) {
        cout<<"x = np.array([";
        for (int j = 0; j < zz; ++j) {
            if(j!=zz-1)
            cout<<answer[0][j]<<",";
            else
                cout<<answer[0][zz-1]<<"]) \n";
        }
        cout<<"y = np.array([";
        for (int j = 0; j < zz; ++j) {
            if(j!=zz-1)
                cout<<answer[i][j]<<",";
            else
                cout<<answer[i][zz-1]<<"])\n";
        }
        cout<<"figure(figsize=(15, 10), dpi=80)"<<endl;

        cout<<"plt.plot(x, y, color='black', linestyle='dashed', marker='o', markerfacecolor='red', markersize=10)"<<endl;
        cout<<"plt.xticks(np.arange(0, max(x)+10, 10))"<<endl;
        cout<<"plt.yticks(np.arange(0, 170, 10))"<<endl;
        cout<<"plt.title(\"Number of Users vs Drop rate \")"<<endl;
        cout<<"plt.xlabel(\"Number of Users\")"<<endl;
        cout<<"plt.ylabel(\"Drop rate \")"<<endl;
        cout<<"plt.savefig(\'droprate.png\')"<<endl;
        cout<<"plt.show()"<<endl;
    }


    return 0;
}


