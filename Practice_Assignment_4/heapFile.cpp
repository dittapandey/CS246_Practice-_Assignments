#include<bits/stdc++.h>
using namespace std;
int debug = 0;

class page{
    public:
        int max_size;
        int current_size;                   //stores current size of the file including all bookeeping data, data records everything
        int page_index;
        page* prev_page;
        page* next_page;
        int num_records;
        int add_of_free_sp;
        vector<pair<int,int>>records;       //contains primary key, size pair
        vector<int>address_of_records;
        page(int m_s, page* p_p = nullptr, page* n_p = nullptr){
            max_size = m_s;
            prev_page = p_p;
            next_page = n_p;
            num_records = 0;
            add_of_free_sp = 0;
            current_size = 16;
            if(prev_page!=nullptr){
                page_index = prev_page->page_index+1;
            } else {
                page_index = 0;
            }
        }

        int insert(int pk, int size){
            // three cases 0: page has appropriate space available, 1: It does not have appropriate space available
            if(current_size + size + 4 > max_size){
                return 1;
            }
            records.push_back({pk, size});
            address_of_records.push_back(add_of_free_sp);
            add_of_free_sp = add_of_free_sp+size;
            num_records++;
            current_size = current_size + size +4;
            return 0;
        }

        int search(int pk){
            // return address of location for found in page else return -1
            for(int i=0;i<records.size();i++){
                if(records[i].first == pk){
                    return i;
                }
            }
            return -1;
        }

};

class heap{
    public: 
        int size_page;
        vector<page*>pages;
        page* root;
        page* last_page;
        int num_page;
        heap(int s_p){
            size_page = s_p;
            root=nullptr;
            num_page = 0;
        }
        int insert(int pk, int size){
            // 0: has gotten inserted in the required page, 1: new page is needed to be created
            // returned -1 if insert is not possible at all
            if(root == nullptr){
                page* n_page =  new page(size_page);
                num_page++;
                root = n_page;
                last_page = n_page;
                int response = n_page->insert(pk,size);
                if(response == 1) return -1;
                else {
                    return 0;
                }
            } 
            int response;
            page* t = root;
            while(t!=nullptr){
                if(t->current_size+size+4 <=t->max_size){
                    response = t->insert(pk,size);
                    return 0;
                } else{
                    t=t->next_page;
                }
            }
            if(t==nullptr){
                page* n_page = new page(size_page, last_page);
                num_page++;
                last_page->next_page=n_page;
                last_page = n_page;
                int res = last_page->insert(pk,size);
                if(res == 0) return 0;
                else return -1;
            }
            return -1;
        }

        pair<int,int> search(int pk){
            page* t=root;
            int res;
            while(t!=nullptr){
                res = t->search(pk);
                if(res!=-1) break;
                else {
                    t = t->next_page;
                }
            }
            if(t==nullptr){
                return {-1,-1};
            } //not found
            else {
                return {t->page_index, res};
            }
        }
        
        void status(){
            page* t= root;
            cout<<num_page<<" ";
            while(t!=nullptr){
                cout<<t->num_records<<" ";
                // cout<<"Current size of page: "<<t->current_size<<endl;
                if(debug){
                    cout<<"Records:"<<endl;
                    for(auto x : t->records){
                        cout<<x.first<<" "<<x.second<<endl;
                    }
                    cout<<"----"<<endl;
                }
                t=t->next_page;
            }
            cout<<endl;
        }
};


int main(){
    int p_s;
    cin>>p_s;
    heap* h = new heap(p_s);
    int choice;
    do{
        cin>>choice;
        if(choice == 1){
            int pk, size;
            cin>>size>>pk;
            int res = h->insert(pk,size);
            if(debug){
                if(res == 0) cout<<"insert success"<<endl;
                else cout<<"insert fail"<<endl;
            }
        } else if(choice == 3){
            int pk;
            cin>>pk;
            pair<int,int>res = h->search(pk);
            cout<<res.first<<" "<<res.second<<endl;
        } else if(choice == 2){
            h->status();
        }
    }while(choice!=4);
    return 0;
}
