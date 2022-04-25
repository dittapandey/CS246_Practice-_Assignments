#include<bits/stdc++.h>
using namespace std;
#define pb push_back
typedef pair<int, pair<int,int>> ppi;
class Page{
    public: 
        int maxNumRecords;
        vector<int>records;
        bool filled;
        Page* nextPage;
        Page(int mNR){
            maxNumRecords = mNR;
            filled = false;
            nextPage = nullptr;
        }
        int insert(int record){
            if(records.size() == maxNumRecords){
                filled = true;
                return 0; // page completely filled
            } else {
                records.pb(record);
                return 1; // page not filled yet
            }
        }
        void sort(){
            std::sort(records.begin(), records.end());
        }
        void displayPage(){
            for(int i=0;i<records.size();i++){
                cout<<records[i]<<endl;
            }
            cout<<"---------------------"<<endl;
        }
};

class File{
    public:
        int num_pages;
        vector<Page*>pages;
        vector<Page*>buffer; //last page in this will always be used for ouputting the page if output page is to be considered
        int maxRecordsPerPage;
        int B;
        File(int mRPR, int bufferSize){
            maxRecordsPerPage = mRPR;
            num_pages = 0;
            B = bufferSize;
            for(int i=0;i<bufferSize;i++) buffer.pb(nullptr);
        }
        int insert(int record){
            if(pages.size() == 0){
                Page* nPage = new Page(maxRecordsPerPage);
                pages.pb(nPage);
                int res = nPage->insert(record);
                if(res == 0){
                    cout<<"---------------------------------------------------------"<<endl;
                    cout<<"Unknown error detected while inserting in new Page"<<endl;
                    cout<<"---------------------------------------------------------"<<endl;
                    return 0;
                } else {
                    return 1;
                }
            }
            Page* targetPage = pages[pages.size()-1];
            int res = targetPage->insert(record);
            if(res == 1){
                return 1; // insertion successful
            } else {
                Page* nPage = new Page(maxRecordsPerPage);
                pages.pb(nPage);
                int res = nPage->insert(record);
                if(res == 0){
                    cout<<"---------------------------------------------------------"<<endl;
                    cout<<"Unknown error detected while inserting in new Page"<<endl;
                    cout<<"---------------------------------------------------------"<<endl;
                    return 0;
                } else {
                    return 1;
                }
            }
        }

        // void mergeKSortedPages(vector<Page*>&input, int l, int r,vector<Page*>& output) //l is inclusive r is not inclusive
        // {
        //     if(l>=r) return;
        //     vector<int>indexes(r-l,0);
        //     vector<Page*>temp;
        //      //generating the output page wise
        //     bool f = 1;
        //     for(int i=l;i<r;i++){
        //         input[i]->displayPage();
        //     }
        //     while(f){
        //         int minRecord = INT_MAX;
        //         int minRecordPos = -1;
        //         int minRecordPage = -1;
        //         int count = 0;
        //         // cout<<count<<endl;
        //         for(int i=l;i<r;i++){
        //             if(indexes[i-1] == input[i]->records.size()){
        //                 count++;
        //             }
        //             if(input[i]->records[indexes[i-l]]<minRecord && indexes[i-1]<input[i]->records.size()){
        //                 // cout<<1<<endl;
        //                 minRecord = input[i]->records[indexes[i-l]];
        //                 minRecordPos = indexes[i-l];
        //                 minRecordPage = i;
        //             }
        //         }
        //         if(count == r-l) break;
        //         // cout<<minRecordPage<<endl;
        //         indexes[minRecordPage - l]++;
                // if(output.size() == 0){
                //     Page* npage = new Page(maxRecordsPerPage);
                //     output.pb(npage);
                //     int res = npage->insert(minRecord);
                // } else {
                //     int res = output[output.size()-1]->insert(minRecord);
                //     if(res == 0){
                //         Page* npage = new Page(maxRecordsPerPage);
                //         output.pb(npage);
                //         int res = npage->insert(minRecord);
                //     }
                // }
                
        //     }
        // }
        void mergeKSortedPages(vector<Page*>&arr, int l, int r, vector<Page*>&output)
        {
            // vector<int> output;
        
            // Create a min heap with k heap nodes. Every
            // heap node has first element of an array
            if(l>=r || r>arr.size()) return;
            priority_queue<ppi, vector<ppi>, greater<ppi> > pq;
            for (int i = l; i < r; i++)
                pq.push({ arr[i]->records[0], { i, 0 } });
        
            // Now one by one get the minimum element
            // from min heap and replace it with next
            // element of its array
            while (pq.empty() == false) {
                ppi curr = pq.top();
                pq.pop();
        
                // i ==> Array Number
                // j ==> Index in the array number
                int i = curr.second.first;
                int j = curr.second.second;
        
                if(output.size() == 0){
                    Page* npage = new Page(maxRecordsPerPage);
                    output.pb(npage);
                    int res = npage->insert(curr.first);
                } else {
                    int res = output[output.size()-1]->insert(curr.first);
                    if(res == 0){
                        Page* npage = new Page(maxRecordsPerPage);
                        output.pb(npage);
                        int res = npage->insert(curr.first);
                    }
                }
        
                // The next element belongs to same array as
                // current.
                if (j + 1 < arr[i]->records.size())
                    pq.push({ arr[i]->records[j + 1], { i, j + 1 } });
            }
            
            cout<<"Output for l:"<<l<<" and r:"<<r<<":"<<endl;
            for(int i=0;i<output.size();i++){
                cout<<"Output of Page: "<<i<<endl;
                output[i]->displayPage();
            }
        }

        void sort(){
            int pass = 0;
            vector<Page*>runs; // temporary array to store the linked lists of the runs of pages
            int no_runs = pages.size();
            int size_sorted = 1;
            cout<<"Hello are you working"<<endl;
            while(size_sorted <= pages.size()){
                // cout<<size_sorted<<endl;
                cout<<"Pass is"<<pass<<endl;
                if(pass == 0){
                    for(int i=0;i<pages.size();i++){
                        pages[i]->sort();
                        // pages[i]->displayPage();
                    }
                    
                    for(int i=0;i<pages.size();i+=B){
                        vector<Page*>output; // output buffer can be generated using this array page by page
                        
                        mergeKSortedPages(pages, i, min(i+B, (int)pages.size()),output);
                        for(int j=i;j<min(i+B, (int)pages.size());j++){
                            pages[j] = output[j-i];
                        }
                    }
                    no_runs = pages.size()/B;
                    if(pages.size()%B>0) no_runs+=1;
                    // cout<<"No_runs"<<no_runs<<endl;
                    size_sorted = B;
                } else {
                    size_sorted = size_sorted*(B-1);
                    for(int i=0;i<pages.size();i+=size_sorted-1){
                        vector<Page*>output; // output buffer can be generated using this array page by page
                        mergeKSortedPages(pages, i, min(i+size_sorted-1, (int)pages.size()),output);
                        for(int j=i;j<min(i+size_sorted-1, (int)pages.size());j++){
                            pages[j] = output[j-i];
                        }
                    }
                    
                }
                pass++;
            }
            // cout<<"-----"<<endl;
            // cout<<size_sorted<<endl;
        }

        void displayFile(){
            int record_num = 0;
            for(int i=0;i<pages.size();i++){
                cout<<"--------------------------"<<endl;
                for(int j=0;j<pages[i]->records.size();j++){
                    // cout<<"in File: "<<i<<endl;
                    // record_num++;
                    
                    cout<<pages[i]->records[j]<<endl;
                }
            }
        }

};


int main(){
    int numBufferPages, recordsPerPage, recordsInFile;
    cin>>numBufferPages>>recordsPerPage>>recordsInFile;
    // cout<<numBufferPages<<recordsPerPage<<recordsInFile<<endl;
    File* f = new File(recordsPerPage, numBufferPages);
    for(int i=0;i<recordsInFile;i++){
        int record;
        cin>>record;
        // cout<<record<<endl;
        int res = f->insert(record);
        // cout<<i<<endl;
        if(res == 0){
            cout<<"---------------------------------------------------------------"<<endl;
            cout<<"Error encountered while enetring in file from the main function"<<endl;
            cout<<"---------------------------------------------------------------"<<endl;
        }
    }
    // cout<<f->pages.size()<<endl;
    f->sort();
    // f->displayFile();
}