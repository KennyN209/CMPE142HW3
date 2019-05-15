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
    bool free();
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
    friend istream &operator >>(istream &in_stream, pT&);
    friend ostream &operator <<(ostream &out_stream, pT&);
};

class Process{
public:
    Process();
    int PID;
    bool isCreated;
    bool isTerminated;
    pT* pages;
};
pT slide[20];
pT swapped[50];
Process Processes[50];

//------------------------------------CLASSES-------------------------//

int main()
{
    int PID = 0;
    int vAddress = 0;
    char action;
    int accessTimeStamp = 1;
    int BA;
    int choice = 0;
    int swapIndex = 0;
    int PageIndexSwap = 0;
    int count = 1;
    
    
    ifstream in_stream;
    in_stream.open("memory.dat");
    
    while(choice != /* DISABLES CODE */ (1)
          || choice != 2 || choice != 3)
    {
        cout<<"Choose a Swap Policy: " << endl << "1: FIFO" << endl << "2: LRU" << endl << "3: Random" << endl << endl << "Choose your swap placement scheme: ";
        cin >> choice;
        if(choice == 1 || choice == 2 || choice == 3)
            break;
        
    }
    while (in_stream >> PID)
    {
        in_stream >> action;
        if (action == 'A' || action == 'W' || action == 'F' || action == 'R')
        {
            in_stream >> vAddress;
        }
        
        
        if (action == 'C')
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
        
        if (action == 'A')
        {
            BA = 0;
            swapIndex = 0;
            bool SwapAlgorithmYesNo = false;
            bool SwapAlgorithmHappenedYesNo = false;
            
            for(BA = 0; BA < 20; BA++)
            {
                
                if(!slide[BA].allocated)
                    break;
                if(BA == 19 && slide[BA].allocated) SwapAlgorithmHappenedYesNo = true;
                
            }
            
            if(SwapAlgorithmHappenedYesNo){
                for(BA = 0; BA < 20; BA++){
                    if(!slide[BA].dirty) break;
                    if(BA == 19 && slide[BA].dirty) SwapAlgorithmYesNo = true;
                }
                
            }
            
            if (SwapAlgorithmYesNo)
            {
                if(choice == 1){
                    int LastUsedPage = 0;
                    for(int i = 0; i < 20; i++){
                        if(slide[i].accum < slide[LastUsedPage].accum){
                            LastUsedPage = i;
                        }
                    }
                    PageIndexSwap = LastUsedPage;
                }
                
                if(choice == 2){
                    int pageWithLeastAccessed = 0;
                    for(int i = 0; i < 20; i++){
                        if(slide[i].visited < slide[pageWithLeastAccessed].visited){
                            pageWithLeastAccessed = i;
                        }
                    }
                    PageIndexSwap = pageWithLeastAccessed;
                }
                
                if(choice == 3){
                    BA = rand() % 20 + 0;
                }
                
            }
            for (int i = 0; i < 50; i++)
            {
                if (Processes[i].PID == PID)
                {
                    
                    if(SwapAlgorithmHappenedYesNo)
                    {
                        
                        while (swapped[swapIndex].allocated)
                        {
                            swapIndex++;
                        }
                        
                        swapped[swapIndex].pID = slide[BA].pID;
                        swapped[swapIndex].virtualAd = slide[BA].virtualAd;
                        swapped[swapIndex].allocated = true;
                        swapped[swapIndex].dirty = slide[BA].dirty;
                        swapped[swapIndex].visited = slide[BA].visited;
                        for (int processToSwap = 0; processToSwap < 50;
                             processToSwap++)
                        {
                            if (Processes[processToSwap].PID== slide[BA].pID)
                            {
                                Processes[processToSwap].pages[slide[BA].virtualAd].isSwapped =
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
                        Processes[i].pages[vAddress].accum = count;
                        count++;
                        
                    }
                    
                    
                    else
                    {
                        
                        Processes[i].pages[vAddress].virtualAd = vAddress;
                        Processes[i].pages[vAddress].physicalAd = BA;
                        Processes[i].pages[vAddress].allocated = true;
                        Processes[i].pages[vAddress].accum = count;
                        count++;
                    }
                    
                    slide[BA].pID = PID;
                    slide[BA].virtualAd = vAddress;
                    slide[BA].physicalAd = BA;
                    slide[BA].allocated = true;
                    slide[BA].accum = count;
                    count++;
                    break;
                }
            }
            
            
            
        }
        if (action == 'W')
        {
            bool SwapWrittenwithAlgo = true;
            swapIndex = 0;
            int pageFoundInSwap = 0;
            
            
            
            for (int i = 0; i < 50; i++)
            {
                if(Processes[i].PID == PID){
                    
                    if(!Processes[i].pages[vAddress].allocated){
                        
                        
                        for(int killIndex = 0; killIndex < 20; killIndex++){
                            
                            if(slide[killIndex].pID == PID){
                                slide[killIndex].allocated = false;
                                slide[killIndex].virtualAd = 0;
                                slide[killIndex].dirty = false;
                                slide[killIndex].visited = 0;
                                slide[killIndex].pID = 0;
                            }
                        }
                        
                        for(int killSwapIndex = 0; killSwapIndex < 40; killSwapIndex++){
                            
                            if(swapped[killSwapIndex].pID == PID){
                                swapped[killSwapIndex].allocated = false;
                                swapped[killSwapIndex].virtualAd = 0;
                                swapped[killSwapIndex].dirty = false;
                                swapped[killSwapIndex].visited = 0;
                                swapped[killSwapIndex].pID = 0;
                            }
                        }
                        
                        Processes[i].isCreated = false;
                        Processes[i].isTerminated = true;
                        Processes[i].pages = NULL;
                        break;
                    }
                    
                    
                    
                    else if(Processes[i].pages[vAddress].allocated && Processes[i].pages[vAddress].isSwapped){
                        
                        for(pageFoundInSwap = 0; pageFoundInSwap < 40; pageFoundInSwap++){
                            if(swapped[pageFoundInSwap].pID == PID && swapped[pageFoundInSwap].virtualAd == vAddress)break;
                        }
                        
                        
                        while (swapped[swapIndex].allocated && swapIndex < 40)
                        {
                            swapIndex++;
                        }
                        
                        for(PageIndexSwap = 0; PageIndexSwap < 20; PageIndexSwap++){
                            if(!slide[PageIndexSwap].dirty){
                                SwapWrittenwithAlgo = false;
                                
                                break;
                            }
                        }
                        
                        
                        if(SwapWrittenwithAlgo){
                            cout<<"Swap Rules are needed"<<endl;
                            
                            
                            if(choice == 1){
                                int pageWithLeastAccessed = 0;
                                for(int i = 0; i < 20; i++){
                                    if(slide[i].visited < slide[pageWithLeastAccessed].visited){
                                        pageWithLeastAccessed = i;
                                    }
                                }
                                PageIndexSwap = pageWithLeastAccessed;
                            }
                            
                            if(choice == 2){
                                int pageWithLeastAccessed = 0;
                                for(int i = 0; i < 20; i++){
                                    if(slide[i].visited < slide[pageWithLeastAccessed].visited){
                                        pageWithLeastAccessed = i;
                                    }
                                }
                                PageIndexSwap = pageWithLeastAccessed;
                            }
                            
                            if(choice == 3){
                                PageIndexSwap = rand() % 20 + 0;
                            }
                            
                            
                            
                        }
                        
                        
                        swapped[swapIndex].pID = slide[PageIndexSwap].pID;
                        swapped[swapIndex].physicalAd = slide[PageIndexSwap].physicalAd;
                        swapped[swapIndex].virtualAd = slide[PageIndexSwap].virtualAd;
                        swapped[swapIndex].isSwapped = true;
                        swapped[swapIndex].accum = 0;
                        
                        for(int k = 0; k < 50; k++){
                            if(Processes[k].PID == slide[PageIndexSwap].pID){
                                
                                if(Processes[k].pages[slide[PageIndexSwap].virtualAd].virtualAd == slide[PageIndexSwap].virtualAd){
                                    Processes[k].pages[slide[PageIndexSwap].virtualAd].virtualAd = true;
                                    break;
                                    
                                }
                            }
                            
                        }
                        
                        
                        
                        slide[PageIndexSwap].dirty = true;
                        slide[PageIndexSwap].pID = swapped[pageFoundInSwap].pID;
                        slide[PageIndexSwap].isSwapped = false;
                        slide[PageIndexSwap].allocated = true;
                        slide[PageIndexSwap].physicalAd = swapped[pageFoundInSwap].physicalAd;
                        slide[PageIndexSwap].virtualAd = swapped[pageFoundInSwap].virtualAd;
                        slide[PageIndexSwap].accum = count;
                        
                        
                        
                        for(int i = 0; i < 50; i++){
                            if(Processes[i].PID == swapped[pageFoundInSwap].pID){
                                for(int j = 0; j < 100; j++){
                                    if(Processes[i].pages[j].virtualAd == swapped[pageFoundInSwap].virtualAd){
                                        Processes[i].pages[j].isSwapped = false;
                                        Processes[i].pages[j].physicalAd = PageIndexSwap;
                                        break;
                                    }
                                }
                            }
                            
                        }
                        count++;
                        
                        
                        
                        swapped[pageFoundInSwap].allocated = false;
                        swapped[pageFoundInSwap].virtualAd = 0;
                        swapped[pageFoundInSwap].dirty = false;
                        swapped[pageFoundInSwap].visited = 0;
                        swapped[pageFoundInSwap].pID = 0;
                        
                    }
                    
                    
                    else{
                        Processes[i].pages[vAddress].dirty = true;
                        slide[Processes[i].pages[vAddress].physicalAd].dirty =
                        true;
                        slide[Processes[i].pages[vAddress].physicalAd].visited =
                        accessTimeStamp;
                        accessTimeStamp++;
                        break;
                    }
                }
            }
            
            
            if (action == 'R')
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
                                
                                if (slide[killIndex].pID == PID){
                                    slide[killIndex].allocated = false;
                                    slide[killIndex].virtualAd = 0;
                                    slide[killIndex].dirty = false;
                                    slide[killIndex].visited = 0;
                                    slide[killIndex].pID = 0;
                                }
                            }
                            for(int killSwapIndex = 0; killSwapIndex < 40; killSwapIndex++){
                                
                                if(swapped[killSwapIndex].pID == PID){
                                    swapped[killSwapIndex].allocated = false;
                                    swapped[killSwapIndex].virtualAd = 0;
                                    swapped[killSwapIndex].dirty = false;
                                    swapped[killSwapIndex].visited = 0;
                                    swapped[killSwapIndex].pID = 0;
                                }
                            }
                            
                            Processes[i].isCreated = false;
                            Processes[i].isTerminated = true;
                            Processes[i].pages = NULL;
                            break;
                        }
                        
                        
                        else if(Processes[i].pages[vAddress].allocated && Processes[i].pages[vAddress].isSwapped){
                            
                            for(pageFoundInSwap = 0; pageFoundInSwap < 40; pageFoundInSwap++){
                                if(swapped[pageFoundInSwap].pID == PID && swapped[pageFoundInSwap].virtualAd == vAddress)break;
                            }
                            
                            while (swapped[swapIndex].allocated && swapIndex < 40)
                            {
                                swapIndex++;
                            }
                            
                            
                            for(int swapPageIndex = 0; swapPageIndex < 20; swapPageIndex++){
                                if(!slide[swapPageIndex].dirty){
                                    needSwapForReadAlgo = false;
                                    
                                    break;
                                }
                            }
                            
                            if(needSwapForReadAlgo)
                            {
                                
                                if(choice == 1){
                                    int pageWithLeastAccessed = 0;
                                    for(int i = 0; i < 20; i++){
                                        if(slide[i].visited < slide[pageWithLeastAccessed].visited){
                                            pageWithLeastAccessed = i;
                                        }
                                    }
                                    PageIndexSwap = pageWithLeastAccessed;
                                }
                                //LRU
                                if(choice== 2){
                                    int pageWithLeastAccessed = 0;
                                    for(int i = 0; i < 20; i++){
                                        if(slide[i].visited < slide[pageWithLeastAccessed].visited){
                                            pageWithLeastAccessed = i;
                                        }
                                    }
                                    PageIndexSwap = pageWithLeastAccessed;
                                }
                                
                                if(choice == 3){
                                    PageIndexSwap = rand() % 20 + 0;
                                }
                                
                            }
                            
                            swapped[swapIndex].pID = slide[PageIndexSwap].pID;
                            swapped[swapIndex].physicalAd = slide[PageIndexSwap].physicalAd;
                            swapped[swapIndex].virtualAd = slide[PageIndexSwap].virtualAd;
                            swapped[swapIndex].isSwapped = true;
                            for(int k = 0; k < 50; k++){
                                if(Processes[k].PID == slide[PageIndexSwap].pID){
                                    
                                    if(Processes[k].pages[slide[PageIndexSwap].virtualAd].virtualAd == slide[PageIndexSwap].virtualAd){
                                        Processes[k].pages[slide[PageIndexSwap].virtualAd].virtualAd = true;
                                        break;
                                        
                                    }
                                }
                                
                            }
                            
                            slide[PageIndexSwap].pID = swapped[pageFoundInSwap].pID;
                            slide[PageIndexSwap].isSwapped = false;
                            slide[PageIndexSwap].allocated = true;
                            slide[PageIndexSwap].physicalAd = swapped[pageFoundInSwap].physicalAd;
                            slide[PageIndexSwap].virtualAd = swapped[pageFoundInSwap].virtualAd;
                            slide[PageIndexSwap].accum = count;
                            for(int i = 0; i < 50; i++){
                                if(Processes[i].PID == swapped[pageFoundInSwap].pID){
                                    for(int j = 0; j < 100; j++){
                                        if(Processes[i].pages[j].virtualAd == swapped[pageFoundInSwap].virtualAd){
                                            Processes[i].pages[j].isSwapped = false;
                                            Processes[i].pages[j].physicalAd = PageIndexSwap;
                                            break;
                                        }
                                    }
                                }
                                
                            }
                            count++;
                            
                            
                            
                            swapped[pageFoundInSwap].allocated = false;
                            swapped[pageFoundInSwap].virtualAd = 0;
                            swapped[pageFoundInSwap].dirty = false;
                            swapped[pageFoundInSwap].visited = 0;
                            swapped[pageFoundInSwap].pID = 0;
                            
                        }
                        else{
                            Processes[i].pages[vAddress].visited = accessTimeStamp;
                            slide[Processes[i].pages[vAddress].physicalAd].visited =
                            accessTimeStamp;
                            accessTimeStamp++;
                            break;
                        }
                    }
                    
                    
                }
            }
            
        }
        
        if (action == 'F')
        {
            
            for (int i = 0; i < 50; i++)
            {
                if (Processes[i].PID == PID
                    && Processes[i].pages[vAddress].allocated == true)
                {
                    Processes[i].pages[vAddress].allocated = false;
                    Processes[i].pages[vAddress].isEmpty = true;
                    slide[Processes[i].pages[vAddress].physicalAd].allocated =
                    false;
                    slide[Processes[i].pages[vAddress].physicalAd].virtualAd =
                    0;
                    slide[Processes[i].pages[vAddress].physicalAd].dirty =
                    false;
                    slide[Processes[i].pages[vAddress].physicalAd].visited =
                    0;
                    slide[Processes[i].pages[vAddress].physicalAd].pID =
                    0;
                    
                    break;
                }
                
                else if(Processes[i].PID == PID && !Processes[i].pages[vAddress].allocated){
                    
                    
                    for (int killIndex = 0; killIndex < 20; killIndex++){
                        
                        if (slide[killIndex].pID == PID){
                            slide[killIndex].allocated = false;
                            slide[killIndex].virtualAd = 0;
                            slide[killIndex].dirty = false;
                            slide[killIndex].visited = 0;
                            slide[killIndex].pID = 0;
                        }
                    }
                    for(int killSwapIndex = 0; killSwapIndex < 40; killSwapIndex++){
                        
                        if(swapped[killSwapIndex].pID == PID){
                            swapped[killSwapIndex].allocated = false;
                            swapped[killSwapIndex].virtualAd = 0;
                            swapped[killSwapIndex].dirty = false;
                            swapped[killSwapIndex].visited = 0;
                            swapped[killSwapIndex].pID = 0;
                        }
                    }
                    
                    Processes[i].isCreated = false;
                    Processes[i].isTerminated = true;
                    Processes[i].pages = NULL;
                    break;
                    
                }
            }
            
            for(int freeSwap = 0; freeSwap < 40; freeSwap++){
                if(swapped[freeSwap].pID==PID){
                    swapped[freeSwap].allocated = false;
                    swapped[freeSwap].isEmpty = true;
                    swapped[freeSwap].visited = 0;
                    swapped[freeSwap].pID = 0;
                    swapped[freeSwap].virtualAd = 0;
                    swapped[freeSwap].physicalAd = 0;
                    break;
                }
                
            }
            
        }
        
        if (action == 'T')
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
                
                if (slide[i].pID == PID)
                {
                    slide[i].allocated = false;
                    slide[i].virtualAd = 0;
                    slide[i].dirty = false;
                    slide[i].visited = 0;
                    slide[i].pID = 0;
                    
                }
            }
            
            for(int i = 0; i < 40; i++){
                if(swapped[i].pID == PID){
                    
                    swapped[i].allocated = false;
                    swapped[i].virtualAd = 0;
                    swapped[i].dirty = false;
                    swapped[i].visited = 0;
                    swapped[i].pID = 0;
                    
                }
            }
        }
        
    }
    
    
    cout<<"Swap Placement: " << endl;
    cout << "PID\t\t" << "Virt. Add.\t\t" << "Physical. Add.\t" << "Dirty\t\t" << "Access\t" << endl;
    for (int i = 0; i < 20; i++)
    {
        
        cout << slide[i].pID << "\t\t\t"
        << slide[i].virtualAd << "\t\t\t\t" << i << "\t\t\t\t";
        if (slide[i].dirty == true)
            cout << "yes" << "\t\t\t" << slide[i].visited << endl;
        if (slide[i].dirty == false)
            cout << "no" << "\t\t\t" << slide[i].visited << endl;
        
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
bool pT::free(){
    allocated = false;
    return allocated;
}
