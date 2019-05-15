#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

//---------------------------CLASSES--------------------------------------------------------//
class pageTable{
public:
    pageTable();
    int get_processID();
    bool isDirty();
    bool makeDirty();
    bool setFree();
    friend std::istream &operator >>(std::istream &in_stream, pageTable &page);
    friend std::ostream &operator <<(std::ostream &out_stream, pageTable &page);
    void operator =(pageTable &page);
    int processID; //ID of the page
    int physicalAddress; //what page are we on
    int virtualAddress;
    bool dirty;
    int accessed;
    int FIFOcount;
    bool isFreed;
    bool isAllocated;
    bool inSwapSpace;
};

class Process{
public:
    Process();
    int PID;
    bool isCreated;
    bool isTerminated;
    pageTable* pages;
};
pageTable physicalPages[20];
pageTable swapSpace[40];
Process Processes[50];

//------------------------------------CLASSES-------------------------//

int main()
{

    int PID, VA;
    char status;
    int accessTimeStamp = 1;
    int BA;
    int PageFoundSwap, swapRules, ActionCounter, SpaceIndexSwap, PageIndexSwap = 0;
    int FIFOcounter = 1;


    ifstream in_stream;
    in_stream.open("memory.dat");

    while(swapRules != 1 || swapRules != 2 || swapRules != 3){
        cout<<"Select one of the following: "<<endl << " 1 for Random" << endl << "2 for FIFO" << endl << "3 for LRU" << endl;
        cin >> swapRules;
        if(swapRules == 1 || swapRules == 2 || swapRules == 3)
            break;

    }
    while (in_stream >> PID)
    {


        in_stream >> status;

        if (status == 'A' || status == 'W' || status == 'F' || status == 'R')
        {
            in_stream >> VA;

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

                if(!physicalPages[BA].isAllocated)
                    break;
                if(BA == 19 && physicalPages[BA].isAllocated) SwapAlgorithmHappenedYesNo = true;

            }

            if(SwapAlgorithmHappenedYesNo){
                for(BA = 0; BA < 20; BA++){
                    if(!physicalPages[BA].dirty) break;
                    if(BA == 19 && physicalPages[BA].dirty) SwapAlgorithmYesNo = true;
                }

            }

            if(SwapAlgorithmYesNo){

                cout<<"Use the swap algorithm"<<endl;

                if(swapRules == 1){
                    BA = rand() % 20 + 0;
                }

                if(swapRules == 2){
                    int LastUsedPage = 0;
                    for(int i = 0; i < 20; i++){
                        if(physicalPages[i].FIFOcount < physicalPages[LastUsedPage].FIFOcount){
                            LastUsedPage = i;
                        }
                    }
                    PageIndexSwap = LastUsedPage;
                }

                if(swapRules == 3){
                    int pageWithLeastAccessed = 0;
                    for(int i = 0; i < 20; i++){
                        if(physicalPages[i].accessed < physicalPages[pageWithLeastAccessed].accessed){
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

                        while (swapSpace[SpaceIndexSwap].isAllocated)
                        {
                            SpaceIndexSwap++;
                        }

                        swapSpace[SpaceIndexSwap].processID =
                                physicalPages[BA].processID;
                        swapSpace[SpaceIndexSwap].virtualAddress =
                                physicalPages[BA].virtualAddress;
                        swapSpace[SpaceIndexSwap].isAllocated = true;
                        swapSpace[SpaceIndexSwap].dirty =
                                physicalPages[BA].dirty;
                        swapSpace[SpaceIndexSwap].accessed =
                                physicalPages[BA].accessed;

                        for (int processToSwap = 0; processToSwap < 50;
                             processToSwap++)
                        {
                            if (Processes[processToSwap].PID
                                == physicalPages[BA].processID)
                            {
                                Processes[processToSwap].pages[physicalPages[BA].virtualAddress].inSwapSpace =
                                        true;
                                break;
                            }
                        }

                    }

                    if (Processes[i].pages == NULL)
                    {
                        Processes[i].pages = new pageTable[1000];
                        Processes[i].pages[VA].virtualAddress = VA;
                        Processes[i].pages[VA].physicalAddress = BA;
                        Processes[i].pages[VA].isAllocated = true;
                        Processes[i].pages[VA].FIFOcount = FIFOcounter;
                        FIFOcounter++;

                    }


                    else
                    {

                        Processes[i].pages[VA].virtualAddress = VA;
                        Processes[i].pages[VA].physicalAddress = BA;
                        Processes[i].pages[VA].isAllocated = true;
                        Processes[i].pages[VA].FIFOcount = FIFOcounter;
                        FIFOcounter++;
                    }

                    physicalPages[BA].processID = PID;
                    physicalPages[BA].virtualAddress = VA;
                    physicalPages[BA].physicalAddress = BA;
                    physicalPages[BA].isAllocated = true;
                    physicalPages[BA].FIFOcount = FIFOcounter;
                    FIFOcounter++;
                    break;
                }
            }



        }
        if (status == 'W')
        {
            bool SwappedWritten= false;
            bool SwapWrittenwithAlgo = true;
            SpaceIndexSwap = 0;
            int pageFoundInSwap = 0;



            for (int i = 0; i < 50; i++)
            {
                if(Processes[i].PID == PID){

                    if(!Processes[i].pages[VA].isAllocated){


                        for(int killIndex = 0; killIndex < 20; killIndex++){

                            if(physicalPages[killIndex].processID == PID){
                                physicalPages[killIndex].isAllocated = false;
                                physicalPages[killIndex].virtualAddress = 0;
                                physicalPages[killIndex].dirty = false;
                                physicalPages[killIndex].accessed = 0;
                                physicalPages[killIndex].processID = 0;
                            }
                        }

                        for(int killSwapIndex = 0; killSwapIndex < 40; killSwapIndex++){

                            if(swapSpace[killSwapIndex].processID == PID){
                                swapSpace[killSwapIndex].isAllocated = false;
                                swapSpace[killSwapIndex].virtualAddress = 0;
                                swapSpace[killSwapIndex].dirty = false;
                                swapSpace[killSwapIndex].accessed = 0;
                                swapSpace[killSwapIndex].processID = 0;
                            }
                        }

                        Processes[i].isCreated = false;
                        Processes[i].isTerminated = true;
                        Processes[i].pages = NULL;
                        break;
                    }



                    else if(Processes[i].pages[VA].isAllocated && Processes[i].pages[VA].inSwapSpace){

                        for(pageFoundInSwap = 0; pageFoundInSwap < 40; pageFoundInSwap++){
                            if(swapSpace[pageFoundInSwap].processID == PID && swapSpace[pageFoundInSwap].virtualAddress == VA)break;
                        }


                        while (swapSpace[SpaceIndexSwap].isAllocated && SpaceIndexSwap < 40)
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


                            if(swapRules == 1){
                                PageIndexSwap = rand() % 20 + 0;
                            }

                            if(swapRules == 2){
                                int pageWithLeastAccessed = 0;
                                for(int i = 0; i < 20; i++){
                                    if(physicalPages[i].accessed < physicalPages[pageWithLeastAccessed].accessed){
                                        pageWithLeastAccessed = i;
                                    }
                                }
                                PageIndexSwap = pageWithLeastAccessed;
                            }

                            if(swapRules == 3){
                                int pageWithLeastAccessed = 0;
                                for(int i = 0; i < 20; i++){
                                    if(physicalPages[i].accessed < physicalPages[pageWithLeastAccessed].accessed){
                                        pageWithLeastAccessed = i;
                                    }
                                }
                                PageIndexSwap = pageWithLeastAccessed;
                            }



                        }




                        swapSpace[SpaceIndexSwap].processID = physicalPages[PageIndexSwap].processID;
                        swapSpace[SpaceIndexSwap].physicalAddress = physicalPages[PageIndexSwap].physicalAddress;
                        swapSpace[SpaceIndexSwap].virtualAddress = physicalPages[PageIndexSwap].virtualAddress;
                        swapSpace[SpaceIndexSwap].inSwapSpace = true;
                        swapSpace[SpaceIndexSwap].FIFOcount = 0;

                        for(int k = 0; k < 50; k++){
                            if(Processes[k].PID == physicalPages[PageIndexSwap].processID){

                                if(Processes[k].pages[physicalPages[PageIndexSwap].virtualAddress].virtualAddress == physicalPages[PageIndexSwap].virtualAddress){
                                    Processes[k].pages[physicalPages[PageIndexSwap].virtualAddress].virtualAddress = true;
                                    break;

                                }
                            }

                        }



                        physicalPages[PageIndexSwap].dirty = true;
                        physicalPages[PageIndexSwap].processID = swapSpace[pageFoundInSwap].processID;
                        physicalPages[PageIndexSwap].inSwapSpace = false;
                        physicalPages[PageIndexSwap].isAllocated = true;
                        physicalPages[PageIndexSwap].physicalAddress = swapSpace[pageFoundInSwap].physicalAddress;
                        physicalPages[PageIndexSwap].virtualAddress = swapSpace[pageFoundInSwap].virtualAddress;
                        physicalPages[PageIndexSwap].FIFOcount = FIFOcounter;



                        for(int i = 0; i < 50; i++){
                            if(Processes[i].PID == swapSpace[pageFoundInSwap].processID){
                                for(int j = 0; j < 100; j++){
                                    if(Processes[i].pages[j].virtualAddress == swapSpace[pageFoundInSwap].virtualAddress){
                                        Processes[i].pages[j].inSwapSpace = false;
                                        Processes[i].pages[j].physicalAddress = PageIndexSwap;
                                        break;
                                    }
                                }
                            }

                        }
                        FIFOcounter++;



                        swapSpace[pageFoundInSwap].isAllocated = false;
                        swapSpace[pageFoundInSwap].virtualAddress = 0;
                        swapSpace[pageFoundInSwap].dirty = false;
                        swapSpace[pageFoundInSwap].accessed = 0;
                        swapSpace[pageFoundInSwap].processID = 0;

                    }


                    else{
                        Processes[i].pages[VA].dirty = true;
                        physicalPages[Processes[i].pages[VA].physicalAddress].dirty =
                                true;
                        physicalPages[Processes[i].pages[VA].physicalAddress].accessed =
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
                        if(!Processes[i].pages[VA].isAllocated){

                            for(int killIndex = 0; killIndex < 20; killIndex++){

                                if (physicalPages[killIndex].processID == PID){
                                    physicalPages[killIndex].isAllocated = false;
                                    physicalPages[killIndex].virtualAddress = 0;
                                    physicalPages[killIndex].dirty = false;
                                    physicalPages[killIndex].accessed = 0;
                                    physicalPages[killIndex].processID = 0;
                                }
                            }
                            for(int killSwapIndex = 0; killSwapIndex < 40; killSwapIndex++){

                                if(swapSpace[killSwapIndex].processID == PID){
                                    swapSpace[killSwapIndex].isAllocated = false;
                                    swapSpace[killSwapIndex].virtualAddress = 0;
                                    swapSpace[killSwapIndex].dirty = false;
                                    swapSpace[killSwapIndex].accessed = 0;
                                    swapSpace[killSwapIndex].processID = 0;
                                }
                            }

                            Processes[i].isCreated = false;
                            Processes[i].isTerminated = true;
                            Processes[i].pages = NULL;
                            break;
                        }


                        else if(Processes[i].pages[VA].isAllocated && Processes[i].pages[VA].inSwapSpace){

                            for(pageFoundInSwap = 0; pageFoundInSwap < 40; pageFoundInSwap++){
                                if(swapSpace[pageFoundInSwap].processID == PID && swapSpace[pageFoundInSwap].virtualAddress == VA)break;
                            }

                            while (swapSpace[SpaceIndexSwap].isAllocated && SpaceIndexSwap < 40)
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




                                if(swapRules == 1){
                                    PageIndexSwap = rand() % 20 + 0;
                                }

                                if(swapRules == 2){
                                    int pageWithLeastAccessed = 0;
                                    for(int i = 0; i < 20; i++){
                                        if(physicalPages[i].accessed < physicalPages[pageWithLeastAccessed].accessed){
                                            pageWithLeastAccessed = i;
                                        }
                                    }
                                    PageIndexSwap = pageWithLeastAccessed;
                                }
                                //LRU
                                if(swapRules == 3){
                                    int pageWithLeastAccessed = 0;
                                    for(int i = 0; i < 20; i++){
                                        if(physicalPages[i].accessed < physicalPages[pageWithLeastAccessed].accessed){
                                            pageWithLeastAccessed = i;
                                        }
                                    }
                                    PageIndexSwap = pageWithLeastAccessed;
                                }

                            }

                            swapSpace[SpaceIndexSwap].processID = physicalPages[PageIndexSwap].processID;
                            swapSpace[SpaceIndexSwap].physicalAddress = physicalPages[PageIndexSwap].physicalAddress;
                            swapSpace[SpaceIndexSwap].virtualAddress = physicalPages[PageIndexSwap].virtualAddress;
                            swapSpace[SpaceIndexSwap].inSwapSpace = true;
                            for(int k = 0; k < 50; k++){
                                if(Processes[k].PID == physicalPages[PageIndexSwap].processID){

                                    if(Processes[k].pages[physicalPages[PageIndexSwap].virtualAddress].virtualAddress == physicalPages[PageIndexSwap].virtualAddress){
                                        Processes[k].pages[physicalPages[PageIndexSwap].virtualAddress].virtualAddress = true;
                                        break;

                                    }
                                }

                            }

                            physicalPages[PageIndexSwap].processID = swapSpace[pageFoundInSwap].processID;
                            physicalPages[PageIndexSwap].inSwapSpace = false;
                            physicalPages[PageIndexSwap].isAllocated = true;
                            physicalPages[PageIndexSwap].physicalAddress = swapSpace[pageFoundInSwap].physicalAddress;
                            physicalPages[PageIndexSwap].virtualAddress = swapSpace[pageFoundInSwap].virtualAddress;
                            physicalPages[PageIndexSwap].FIFOcount = FIFOcounter;
                            for(int i = 0; i < 50; i++){
                                if(Processes[i].PID == swapSpace[pageFoundInSwap].processID){
                                    for(int j = 0; j < 100; j++){
                                        if(Processes[i].pages[j].virtualAddress == swapSpace[pageFoundInSwap].virtualAddress){
                                            Processes[i].pages[j].inSwapSpace = false;
                                            Processes[i].pages[j].physicalAddress = PageIndexSwap;
                                            break;
                                        }
                                    }
                                }

                            }
                            FIFOcounter++;



                            swapSpace[pageFoundInSwap].isAllocated = false;
                            swapSpace[pageFoundInSwap].virtualAddress = 0;
                            swapSpace[pageFoundInSwap].dirty = false;
                            swapSpace[pageFoundInSwap].accessed = 0;
                            swapSpace[pageFoundInSwap].processID = 0;

                        }
                        else{
                            Processes[i].pages[VA].accessed = accessTimeStamp;
                            physicalPages[Processes[i].pages[VA].physicalAddress].accessed =
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
                    && Processes[i].pages[VA].isAllocated == true)
                {
                    Processes[i].pages[VA].isAllocated = false;
                    Processes[i].pages[VA].isFreed = true;
                    physicalPages[Processes[i].pages[VA].physicalAddress].isAllocated =
                            false;
                    physicalPages[Processes[i].pages[VA].physicalAddress].virtualAddress =
                            0;
                    physicalPages[Processes[i].pages[VA].physicalAddress].dirty =
                            false;
                    physicalPages[Processes[i].pages[VA].physicalAddress].accessed =
                            0;
                    physicalPages[Processes[i].pages[VA].physicalAddress].processID =
                            0;

                    break;
                }

                else if(Processes[i].PID == PID && !Processes[i].pages[VA].isAllocated){


                    for (int killIndex = 0; killIndex < 20; killIndex++){

                        if (physicalPages[killIndex].processID == PID){
                            physicalPages[killIndex].isAllocated = false;
                            physicalPages[killIndex].virtualAddress = 0;
                            physicalPages[killIndex].dirty = false;
                            physicalPages[killIndex].accessed = 0;
                            physicalPages[killIndex].processID = 0;
                        }
                    }
                    for(int killSwapIndex = 0; killSwapIndex < 40; killSwapIndex++){

                        if(swapSpace[killSwapIndex].processID == PID){
                            swapSpace[killSwapIndex].isAllocated = false;
                            swapSpace[killSwapIndex].virtualAddress = 0;
                            swapSpace[killSwapIndex].dirty = false;
                            swapSpace[killSwapIndex].accessed = 0;
                            swapSpace[killSwapIndex].processID = 0;
                        }
                    }

                    Processes[i].isCreated = false;
                    Processes[i].isTerminated = true;
                    Processes[i].pages = NULL;
                    break;

                }
            }

            for(int freeSwap = 0; freeSwap < 40; freeSwap++){
                if(swapSpace[freeSwap].processID==PID){
                    swapSpace[freeSwap].isAllocated = false;
                    swapSpace[freeSwap].isFreed = true;
                    swapSpace[freeSwap].accessed = 0;
                    swapSpace[freeSwap].processID = 0;
                    swapSpace[freeSwap].virtualAddress = 0;
                    swapSpace[freeSwap].physicalAddress = 0;
                    break;
                }

            }

        }

        if (status == 'T')
        {

            for (int i = 0; i < 50; i++)
            {
                if (Processes[i].PID == PID
                    && Processes[i].pages[VA].isAllocated == true)
                {

                    Processes[i].isCreated = false;
                    Processes[i].isTerminated = true;
                    Processes[i].pages = NULL;
                    break;
                }

            }


            for (int i = 0; i < 20; i++){

                if (physicalPages[i].processID == PID)
                {
                    physicalPages[i].isAllocated = false;
                    physicalPages[i].virtualAddress = 0;
                    physicalPages[i].dirty = false;
                    physicalPages[i].accessed = 0;
                    physicalPages[i].processID = 0;

                }
            }

            for(int i = 0; i < 40; i++){
                if(swapSpace[i].processID == PID){

                    swapSpace[i].isAllocated = false;
                    swapSpace[i].virtualAddress = 0;
                    swapSpace[i].dirty = false;
                    swapSpace[i].accessed = 0;
                    swapSpace[i].processID = 0;

                }
            }
        }

    }


    cout<<"Physical Memory" << "Dirty?\t"<< "PID\t" << "BA\t" << "Access\t" << "VA\t" << endl;
    cout << "PID\t" << "VA\t" << "BA\t" << "Dirty?\t" << "Access\t" << endl;
    cout << "___________________________________________________________"
         << endl;
    for (int i = 0; i < 20; i++)
    {

        cout << physicalPages[i].processID << "\t"
             << physicalPages[i].virtualAddress << "\t" << i << "\t";
        if (physicalPages[i].dirty == true)
            cout << "Yes" << "\t" << physicalPages[i].accessed << endl;
        if (physicalPages[i].dirty == false)
            cout << "No" << "\t" << physicalPages[i].accessed << endl;

    }

    cout<<"SWAP SPACE"<<endl;
    cout << "PID\t" << "VA\t" << "Dirty?\t" << "Access\t" << endl;
    cout << "___________________________________________________________"<<endl;
    for (int i = 0; i < 20; i++)
    {
        cout << swapSpace[i].processID << "\t"
             << swapSpace[i].virtualAddress << "\t";
        if (swapSpace[i].dirty == true)
            cout << "Yes" << "\t" << physicalPages[i].accessed << endl;
        if (swapSpace[i].dirty == false)
            cout << "No" << "\t" << physicalPages[i].accessed << endl;
    }

}

Process::Process(){
    PID = 0;
    isCreated = false;
    isTerminated = false;
    pages = NULL;

}
pageTable::pageTable(){
    processID = 0;
    dirty = false;
    isAllocated = false;
    accessed = 0;
    physicalAddress = 0;
    virtualAddress = 0;
    isFreed = false;
    inSwapSpace = false;
    FIFOcount = 0;

}

istream &operator>>(istream &in_stream, pageTable &page){
    in_stream >> page.processID;
    in_stream >> page.virtualAddress;
    in_stream >> page.physicalAddress;
    return in_stream;
}

ostream &operator<<(ostream &out_stream, pageTable &page){
    out_stream << page.processID<<"\t";
    out_stream << page.virtualAddress<<"\t";
    out_stream << page.physicalAddress<<"\t";
    return out_stream;
}


void pageTable::operator=(pageTable &page){
    processID = page.processID;
    dirty = page.dirty;
    isAllocated = page.isAllocated;
    accessed = page.accessed;
    physicalAddress = page.physicalAddress;
    virtualAddress = page.virtualAddress;
}
int pageTable::get_processID(){
    return processID;
}
bool pageTable::isDirty(){
    return dirty;
}
bool pageTable::makeDirty(){
    dirty = true;
    return dirty;
}
bool pageTable::setFree(){
    isAllocated = false;
    return isAllocated;
}