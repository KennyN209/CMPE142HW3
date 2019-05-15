#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

//---------------------------CLASSES--------------------------------------------------------//
class pT{
public:
    pT();
    int getPID();
    bool isDirty();
    bool write();
    bool Free();
    void operator =(pT&);
    int pID; 
    int physicalAd;
    int virtualAd;
    bool dirty;
    int visited;
    int accum;
    bool isEmpty;
    bool allocated;
    bool isSwapped;
    friend istream &operator >>(std::istream &in_stream, pT&);
    friend ostream &operator <<(std::ostream &out_stream, pT&);
};

class Process{
public:
    Process();
    int PID;
    bool isCreated;
    bool isTerminated;
    pT* pages;
};
pT physicalPages[20];
pT swapSpace[50];
Process Processes[50];

//------------------------------------CLASSES-------------------------//

int main()
{
    int PID = 0;
    int vAddress = 0;
    char status;
    int accessTimeStamp = 1;
    int BA;
    int choice = 0;
    int SpaceIndexSwap = 0;
    int PageIndexSwap = 0;
    int FIFOcounter = 1;
    
    
    ifstream in_stream;
    in_stream.open("memory.dat");
    
    while(choice != /* DISABLES CODE */ (1) || choice != 2 || choice != 3)
    {
        cout<<"Choose a Swap Policy: "<<endl << "1: Random" << endl << "2: FIFO" << endl << "3: LRU" << endl << endl << "Choice: ";
        cin >> choice;
        if(choice == 1 || choice == 2 || choice == 3)
            break;
        
    }
    while (in_stream >> PID)
    {
        
        
        in_stream >> status;
        
        if (status == 'A' || status == 'W' || status == 'F' || status == 'R')
        {
            in_stream >> vAddress;
            
        }
        
        
        if (status == 'C')
        {
            int ProcessIndexNew;
            bool TerminatedProcess = false;
            for(ProcessIndexNew = 0; ProcessIndexNew < 50; ProcessIndexNew++){
                
                if(Processes[ProcessIndexNew].PID == PID && Processes[ProcessIndexNew].isTerminated){TerminatedProcess = true;
                    break;}
                
                
            }
            
            if(!TerminatedProcess){
                for(ProcessIndexNew = 0; ProcessIndexNew < 50; ProcessIndexNew++){
                    
                    if(!Processes[ProcessIndexNew].isCreated) break;
                }
                
            }
            Processes[ProcessIndexNew].PID = PID;
            Processes[ProcessIndexNew].isCreated = true;
            Processes[ProcessIndexNew].isTerminated = false;
            
            
        }
        
        if (status == 'A')
        {
            BA = 0;
            SpaceIndexSwap = 0;
            bool SwapAlgorithmYesNo = false;
            bool SwapAlgorithmHappenedYesNo = false;
            
            for(BA = 0; BA < 20; BA++)
            {
                
                if(!physicalPages[BA].allocated)
                    break;
                if(BA == 19 && physicalPages[BA].allocated) SwapAlgorithmHappenedYesNo = true;
                
            }
            
            if(SwapAlgorithmHappenedYesNo){
                for(BA = 0; BA < 20; BA++){
                    if(!physicalPages[BA].dirty) break;
                    if(BA == 19 && physicalPages[BA].dirty) SwapAlgorithmYesNo = true;
                }
                
            }
            
            if(SwapAlgorithmYesNo){
                
                cout<<"Use the swap algorithm"<<endl;
                
                if(choice == 1){
                    BA = rand() % 20 + 0;
                }
                
                if(choice == 2){
                    int LastUsedPage = 0;
                    for(int i = 0; i < 20; i++){
                        if(physicalPages[i].accum < physicalPages[LastUsedPage].accum){
                            LastUsedPage = i;
                        }
                    }
                    PageIndexSwap = LastUsedPage;
                }
                
                if(choice == 3){
                    int pageWithLeastAccessed = 0;
                    for(int i = 0; i < 20; i++){
                        if(physicalPages[i].visited < physicalPages[pageWithLeastAccessed].visited){
                            pageWithLeastAccessed = i;
                        }
                    }
                    PageIndexSwap = pageWithLeastAccessed;
                }
                
                
                
            }
            for (int i = 0; i < 50; i++)
            {
                if (Processes[i].PID == PID)
                {
                    
                    if(SwapAlgorithmHappenedYesNo)
                    {
                        
                        while (swapSpace[SpaceIndexSwap].allocated)
                        {
                            SpaceIndexSwap++;
                        }
                        
                        swapSpace[SpaceIndexSwap].pID =
                        physicalPages[BA].pID;
                        swapSpace[SpaceIndexSwap].virtualAd =
                        physicalPages[BA].virtualAd;
                        swapSpace[SpaceIndexSwap].allocated = true;
                        swapSpace[SpaceIndexSwap].dirty =
                        physicalPages[BA].dirty;
                        swapSpace[SpaceIndexSwap].visited =
                        physicalPages[BA].visited;
                        
                        for (int processToSwap = 0; processToSwap < 50;
                             processToSwap++)
                        {
                            if (Processes[processToSwap].PID
                                == physicalPages[BA].pID)
                            {
                                Processes[processToSwap].pages[physicalPages[BA].virtualAd].isSwapped =
                                true;
                                break;
                            }
                        }
                        
                    }
                    
                    if (Processes[i].pages == NULL)
                    {
                        Processes[i].pages = new pT[1000];
                        Processes[i].pages[vAddress].virtualAd = vAddress;
                        Processes[i].pages[vAddress].physicalAd = BA;
                        Processes[i].pages[vAddress].allocated = true;
                        Processes[i].pages[vAddress].accum = FIFOcounter;
                        FIFOcounter++;
                        
                    }
                    
                    
                    else
                    {
                        
                        Processes[i].pages[vAddress].virtualAd = vAddress;
                        Processes[i].pages[vAddress].physicalAd = BA;
                        Processes[i].pages[vAddress].allocated = true;
                        Processes[i].pages[vAddress].accum = FIFOcounter;
                        FIFOcounter++;
                    }
                    
                    physicalPages[BA].pID = PID;
                    physicalPages[BA].virtualAd = vAddress;
                    physicalPages[BA].physicalAd = BA;
                    physicalPages[BA].allocated = true;
                    physicalPages[BA].accum = FIFOcounter;
                    FIFOcounter++;
                    break;
                }
            }
            
            
            
        }
        if (status == 'W')
        {
            bool SwapWrittenwithAlgo = true;
            SpaceIndexSwap = 0;
            int pageFoundInSwap = 0;
            
            
            
            for (int i = 0; i < 50; i++)
            {
                if(Processes[i].PID == PID){
                    
                    if(!Processes[i].pages[vAddress].allocated){
                        
                        
                        for(int killIndex = 0; killIndex < 20; killIndex++){
                            
                            if(physicalPages[killIndex].pID == PID){
                                physicalPages[killIndex].allocated = false;
                                physicalPages[killIndex].virtualAd = 0;
                                physicalPages[killIndex].dirty = false;
                                physicalPages[killIndex].visited = 0;
                                physicalPages[killIndex].pID = 0;
                            }
                        }
                        
                        for(int killSwapIndex = 0; killSwapIndex < 40; killSwapIndex++){
                            
                            if(swapSpace[killSwapIndex].pID == PID){
                                swapSpace[killSwapIndex].allocated = false;
                                swapSpace[killSwapIndex].virtualAd = 0;
                                swapSpace[killSwapIndex].dirty = false;
                                swapSpace[killSwapIndex].visited = 0;
                                swapSpace[killSwapIndex].pID = 0;
                            }
                        }
                        
                        Processes[i].isCreated = false;
                        Processes[i].isTerminated = true;
                        Processes[i].pages = NULL;
                        break;
                    }
                    
                    
                    
                    else if(Processes[i].pages[vAddress].allocated && Processes[i].pages[vAddress].isSwapped){
                        
                        for(pageFoundInSwap = 0; pageFoundInSwap < 40; pageFoundInSwap++){
                            if(swapSpace[pageFoundInSwap].pID == PID && swapSpace[pageFoundInSwap].virtualAd == vAddress)break;
                        }
                        
                        
                        while (swapSpace[SpaceIndexSwap].allocated && SpaceIndexSwap < 40)
                        {
                            SpaceIndexSwap++;
                        }
                        
                        for(PageIndexSwap = 0; PageIndexSwap < 20; PageIndexSwap++){
                            if(!physicalPages[PageIndexSwap].dirty){
                                SwapWrittenwithAlgo = false;
                                
                                break;
                            }
                        }
                        
                        
                        if(SwapWrittenwithAlgo){
                            cout<<"Swap Rules are needed"<<endl;
                            
                            
                            if(choice == 1){
                                PageIndexSwap = rand() % 20 + 0;
                            }
                            
                            if(choice == 2){
                                int pageWithLeastAccessed = 0;
                                for(int i = 0; i < 20; i++){
                                    if(physicalPages[i].visited < physicalPages[pageWithLeastAccessed].visited){
                                        pageWithLeastAccessed = i;
                                    }
                                }
                                PageIndexSwap = pageWithLeastAccessed;
                            }
                            
                            if(choice == 3){
                                int pageWithLeastAccessed = 0;
                                for(int i = 0; i < 20; i++){
                                    if(physicalPages[i].visited < physicalPages[pageWithLeastAccessed].visited){
                                        pageWithLeastAccessed = i;
                                    }
                                }
                                PageIndexSwap = pageWithLeastAccessed;
                            }
                            
                            
                            
                        }
                        
                        
                        
                        
                        swapSpace[SpaceIndexSwap].pID = physicalPages[PageIndexSwap].pID;
                        swapSpace[SpaceIndexSwap].physicalAd = physicalPages[PageIndexSwap].physicalAd;
                        swapSpace[SpaceIndexSwap].virtualAd = physicalPages[PageIndexSwap].virtualAd;
                        swapSpace[SpaceIndexSwap].isSwapped = true;
                        swapSpace[SpaceIndexSwap].accum = 0;
                        
                        for(int k = 0; k < 50; k++){
                            if(Processes[k].PID == physicalPages[PageIndexSwap].pID){
                                
                                if(Processes[k].pages[physicalPages[PageIndexSwap].virtualAd].virtualAd == physicalPages[PageIndexSwap].virtualAd){
                                    Processes[k].pages[physicalPages[PageIndexSwap].virtualAd].virtualAd = true;
                                    break;
                                    
                                }
                            }
                            
                        }
                        
                        
                        
                        physicalPages[PageIndexSwap].dirty = true;
                        physicalPages[PageIndexSwap].pID = swapSpace[pageFoundInSwap].pID;
                        physicalPages[PageIndexSwap].isSwapped = false;
                        physicalPages[PageIndexSwap].allocated = true;
                        physicalPages[PageIndexSwap].physicalAd = swapSpace[pageFoundInSwap].physicalAd;
                        physicalPages[PageIndexSwap].virtualAd = swapSpace[pageFoundInSwap].virtualAd;
                        physicalPages[PageIndexSwap].accum = FIFOcounter;
                        
                        
                        
                        for(int i = 0; i < 50; i++){
                            if(Processes[i].PID == swapSpace[pageFoundInSwap].pID){
                                for(int j = 0; j < 100; j++){
                                    if(Processes[i].pages[j].virtualAd == swapSpace[pageFoundInSwap].virtualAd){
                                        Processes[i].pages[j].isSwapped = false;
                                        Processes[i].pages[j].physicalAd = PageIndexSwap;
                                        break;
                                    }
                                }
                            }
                            
                        }
                        FIFOcounter++;
                        
                        
                        
                        swapSpace[pageFoundInSwap].allocated = false;
                        swapSpace[pageFoundInSwap].virtualAd = 0;
                        swapSpace[pageFoundInSwap].dirty = false;
                        swapSpace[pageFoundInSwap].visited = 0;
                        swapSpace[pageFoundInSwap].pID = 0;
                        
                    }
                    
                    
                    else{
                        Processes[i].pages[vAddress].dirty = true;
                        physicalPages[Processes[i].pages[vAddress].physicalAd].dirty =
                        true;
                        physicalPages[Processes[i].pages[vAddress].physicalAd].visited =
                        accessTimeStamp;
                        accessTimeStamp++;
                        break;
                    }
                }
            }
            
            
            if (status == 'R')
            {
                
                
                bool needSwapForReadAlgo = true;
                int pageFoundInSwap = 0;
                pageFoundInSwap = 0;
                PageIndexSwap = 0;
                
                for (int i = 0; i < 50; i++)
                {
                    if(Processes[i].PID == PID){
                        if(!Processes[i].pages[vAddress].allocated){
                            
                            for(int killIndex = 0; killIndex < 20; killIndex++){
                                
                                if (physicalPages[killIndex].pID == PID){
                                    physicalPages[killIndex].allocated = false;
                                    physicalPages[killIndex].virtualAd = 0;
                                    physicalPages[killIndex].dirty = false;
                                    physicalPages[killIndex].visited = 0;
                                    physicalPages[killIndex].pID = 0;
                                }
                            }
                            for(int killSwapIndex = 0; killSwapIndex < 40; killSwapIndex++){
                                
                                if(swapSpace[killSwapIndex].pID == PID){
                                    swapSpace[killSwapIndex].allocated = false;
                                    swapSpace[killSwapIndex].virtualAd = 0;
                                    swapSpace[killSwapIndex].dirty = false;
                                    swapSpace[killSwapIndex].visited = 0;
                                    swapSpace[killSwapIndex].pID = 0;
                                }
                            }
                            
                            Processes[i].isCreated = false;
                            Processes[i].isTerminated = true;
                            Processes[i].pages = NULL;
                            break;
                        }
                        
                        
                        else if(Processes[i].pages[vAddress].allocated && Processes[i].pages[vAddress].isSwapped){
                            
                            for(pageFoundInSwap = 0; pageFoundInSwap < 40; pageFoundInSwap++){
                                if(swapSpace[pageFoundInSwap].pID == PID && swapSpace[pageFoundInSwap].virtualAd == vAddress)break;
                            }
                            
                            while (swapSpace[SpaceIndexSwap].allocated && SpaceIndexSwap < 40)
                            {
                                SpaceIndexSwap++;
                            }
                            
                            
                            for(int swapPageIndex = 0; swapPageIndex < 20; swapPageIndex++){
                                if(!physicalPages[swapPageIndex].dirty){
                                    needSwapForReadAlgo = false;
                                    
                                    break;
                                }
                            }
                            
                            if(needSwapForReadAlgo){
                                
                                cout<<"need swap policy"<<endl;
                                
                                
                                
                                
                                if(choice == 1){
                                    PageIndexSwap = rand() % 20 + 0;
                                }
                                
                                if(choice == 2){
                                    int pageWithLeastAccessed = 0;
                                    for(int i = 0; i < 20; i++){
                                        if(physicalPages[i].visited < physicalPages[pageWithLeastAccessed].visited){
                                            pageWithLeastAccessed = i;
                                        }
                                    }
                                    PageIndexSwap = pageWithLeastAccessed;
                                }
                                //LRU
                                if(choice== 3){
                                    int pageWithLeastAccessed = 0;
                                    for(int i = 0; i < 20; i++){
                                        if(physicalPages[i].visited < physicalPages[pageWithLeastAccessed].visited){
                                            pageWithLeastAccessed = i;
                                        }
                                    }
                                    PageIndexSwap = pageWithLeastAccessed;
                                }
                                
                            }
                            
                            swapSpace[SpaceIndexSwap].pID = physicalPages[PageIndexSwap].pID;
                            swapSpace[SpaceIndexSwap].physicalAd = physicalPages[PageIndexSwap].physicalAd;
                            swapSpace[SpaceIndexSwap].virtualAd = physicalPages[PageIndexSwap].virtualAd;
                            swapSpace[SpaceIndexSwap].isSwapped = true;
                            for(int k = 0; k < 50; k++){
                                if(Processes[k].PID == physicalPages[PageIndexSwap].pID){
                                    
                                    if(Processes[k].pages[physicalPages[PageIndexSwap].virtualAd].virtualAd == physicalPages[PageIndexSwap].virtualAd){
                                        Processes[k].pages[physicalPages[PageIndexSwap].virtualAd].virtualAd = true;
                                        break;
                                        
                                    }
                                }
                                
                            }
                            
                            physicalPages[PageIndexSwap].pID = swapSpace[pageFoundInSwap].pID;
                            physicalPages[PageIndexSwap].isSwapped = false;
                            physicalPages[PageIndexSwap].allocated = true;
                            physicalPages[PageIndexSwap].physicalAd = swapSpace[pageFoundInSwap].physicalAd;
                            physicalPages[PageIndexSwap].virtualAd = swapSpace[pageFoundInSwap].virtualAd;
                            physicalPages[PageIndexSwap].accum = FIFOcounter;
                            for(int i = 0; i < 50; i++){
                                if(Processes[i].PID == swapSpace[pageFoundInSwap].pID){
                                    for(int j = 0; j < 100; j++){
                                        if(Processes[i].pages[j].virtualAd == swapSpace[pageFoundInSwap].virtualAd){
                                            Processes[i].pages[j].isSwapped = false;
                                            Processes[i].pages[j].physicalAd = PageIndexSwap;
                                            break;
                                        }
                                    }
                                }
                                
                            }
                            FIFOcounter++;
                            
                            
                            
                            swapSpace[pageFoundInSwap].allocated = false;
                            swapSpace[pageFoundInSwap].virtualAd = 0;
                            swapSpace[pageFoundInSwap].dirty = false;
                            swapSpace[pageFoundInSwap].visited = 0;
                            swapSpace[pageFoundInSwap].pID = 0;
                            
                        }
                        else{
                            Processes[i].pages[vAddress].visited = accessTimeStamp;
                            physicalPages[Processes[i].pages[vAddress].physicalAd].visited =
                            accessTimeStamp;
                            accessTimeStamp++;
                            break;
                        }
                    }
                    
                    
                }
            }
            
        }
        
        if (status == 'F')
        {
            
            for (int i = 0; i < 50; i++)
            {
                if (Processes[i].PID == PID
                    && Processes[i].pages[vAddress].allocated == true)
                {
                    Processes[i].pages[vAddress].allocated = false;
                    Processes[i].pages[vAddress].isEmpty = true;
                    physicalPages[Processes[i].pages[vAddress].physicalAd].allocated =
                    false;
                    physicalPages[Processes[i].pages[vAddress].physicalAd].virtualAd =
                    0;
                    physicalPages[Processes[i].pages[vAddress].physicalAd].dirty =
                    false;
                    physicalPages[Processes[i].pages[vAddress].physicalAd].visited =
                    0;
                    physicalPages[Processes[i].pages[vAddress].physicalAd].pID =
                    0;
                    
                    break;
                }
                
                else if(Processes[i].PID == PID && !Processes[i].pages[vAddress].allocated){
                    
                    
                    for (int killIndex = 0; killIndex < 20; killIndex++){
                        
                        if (physicalPages[killIndex].pID == PID){
                            physicalPages[killIndex].allocated = false;
                            physicalPages[killIndex].virtualAd = 0;
                            physicalPages[killIndex].dirty = false;
                            physicalPages[killIndex].visited = 0;
                            physicalPages[killIndex].pID = 0;
                        }
                    }
                    for(int killSwapIndex = 0; killSwapIndex < 40; killSwapIndex++){
                        
                        if(swapSpace[killSwapIndex].pID == PID){
                            swapSpace[killSwapIndex].allocated = false;
                            swapSpace[killSwapIndex].virtualAd = 0;
                            swapSpace[killSwapIndex].dirty = false;
                            swapSpace[killSwapIndex].visited = 0;
                            swapSpace[killSwapIndex].pID = 0;
                        }
                    }
                    
                    Processes[i].isCreated = false;
                    Processes[i].isTerminated = true;
                    Processes[i].pages = NULL;
                    break;
                    
                }
            }
            
            for(int freeSwap = 0; freeSwap < 40; freeSwap++){
                if(swapSpace[freeSwap].pID==PID){
                    swapSpace[freeSwap].allocated = false;
                    swapSpace[freeSwap].isEmpty = true;
                    swapSpace[freeSwap].visited = 0;
                    swapSpace[freeSwap].pID = 0;
                    swapSpace[freeSwap].virtualAd = 0;
                    swapSpace[freeSwap].physicalAd = 0;
                    break;
                }
                
            }
            
        }
        
        if (status == 'T')
        {
            
            for (int i = 0; i < 50; i++)
            {
                if (Processes[i].PID == PID
                    && Processes[i].pages[vAddress].allocated == true)
                {
                    
                    Processes[i].isCreated = false;
                    Processes[i].isTerminated = true;
                    Processes[i].pages = NULL;
                    break;
                }
                
            }
            
            
            for (int i = 0; i < 20; i++){
                
                if (physicalPages[i].pID == PID)
                {
                    physicalPages[i].allocated = false;
                    physicalPages[i].virtualAd = 0;
                    physicalPages[i].dirty = false;
                    physicalPages[i].visited = 0;
                    physicalPages[i].pID = 0;
                    
                }
            }
            
            for(int i = 0; i < 40; i++){
                if(swapSpace[i].pID == PID){
                    
                    swapSpace[i].allocated = false;
                    swapSpace[i].virtualAd = 0;
                    swapSpace[i].dirty = false;
                    swapSpace[i].visited = 0;
                    swapSpace[i].pID = 0;
                    
                }
            }
        }
        
    }
    
    
    cout<<"Physical Memory" << endl;
    cout << "PID\t" << "VA\t" << "BA\t" << "Dirty\t" << "Access\t" << endl;
    for (int i = 0; i < 20; i++)
    {
        
        cout << physicalPages[i].pID << "\t"
        << physicalPages[i].virtualAd << "\t" << i << "\t\t";
        if (physicalPages[i].dirty == true)
            cout << "Yes" << "\t" << physicalPages[i].visited << endl;
        if (physicalPages[i].dirty == false)
            cout << "No" << "\t" << physicalPages[i].visited << endl;
        
    }
    
    cout<<"SWAP SPACE"<<endl;
    cout << "PID\t" << "VA\t" << "Dirty\t" << "Access\t" << endl;
    for (int i = 0; i < 20; i++)
    {
        cout << swapSpace[i].pID << "\t"
        << swapSpace[i].virtualAd << "\t";
        if (swapSpace[i].dirty == true)
            cout << "Yes" << "\t" << physicalPages[i].visited << endl;
        if (swapSpace[i].dirty == false)
            cout << "No" << "\t\t" << physicalPages[i].visited << endl;
    }
    
}

Process::Process(){
    PID = 0;
    isCreated = false;
    isTerminated = false;
    pages = NULL;
    
}
pT::pT(){
    pID = 0;
    dirty = false;
    allocated = false;
    visited = 0;
    physicalAd = 0;
    virtualAd = 0;
    isEmpty = false;
    isSwapped = false;
    accum = 0;
    
}

istream &operator>>(istream &in_stream, pT &page){
    in_stream >> page.pID;
    in_stream >> page.virtualAd;
    in_stream >> page.physicalAd;
    return in_stream;
}

ostream &operator<<(ostream &out_stream, pT &page){
    out_stream << page.pID<<"\t";
    out_stream << page.virtualAd<<"\t";
    out_stream << page.physicalAd<<"\t";
    return out_stream;
}


void pT::operator=(pT &page){
    pID = page.pID;
    dirty = page.dirty;
    allocated = page.allocated;
    visited = page.visited;
    physicalAd = page.physicalAd;
    virtualAd = page.virtualAd;
}
int pT::getPID(){
    return pID;
}
bool pT::isDirty(){
    return dirty;
}
bool pT::write(){
    dirty = true;
    return dirty;
}
bool pT::Free(){
    allocated = false;
    return allocated;
}
