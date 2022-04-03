#include<bits/stdc++.h>
#include<ctime>
using namespace std;

void print_vector(vector<int>v){
    for(auto x:v){
        cout<<x<<" ";
    }
    cout<<endl;
}



class bucket{
    public:
        time_t time_created;
        int hash_index;
        int local_depth;
        int size;
        vector<int>list;
        bucket(int s, int h_ind, int l_d){
            hash_index=h_ind;
            local_depth=l_d;
            size=s;
            time_created = time(0);
        }

        string insert(int a){
            cout<<"Insert handling at bucket level"<<endl;
            if(this->list.size()==size){
                return "bucket full";
            } else {
                list.push_back(a);
                return "success";
            }
        }

        string del(int a){
            for(int i=0;i<this->list.size();i++){
                if(this->list[i] == a){
                    this->list.erase(this->list.begin()+i);
                    return "success";
                }
            }
            return "value to be deleted not found";
        }
};


bool compare(bucket* x, bucket* y){
    if(x->time_created < y->time_created){
        return 1;
    }
    else return 0;
}

class hash_map{
    public:
        int global_depth;
        vector<bucket*>bucket_list;
        int bucket_size;
        hash_map(int g_d=1, int bucket_size=2){
            this->bucket_size = bucket_size;
            this->global_depth=g_d;
            for(int i=0;i<(1<<g_d);i++){
                bucket* b = new bucket(bucket_size,i,g_d);
                bucket_list.push_back(b);
            }
        }

        void double_directory(){
            cout<<"double directory called"<<endl;
            for(int i=0;i<(1<<global_depth);i++){
                bucket_list.push_back(bucket_list[i]);
            }
            global_depth++;
        }

        string insert(int a){
            int bucket_index = a%(1<<(this->global_depth));
            int ld = bucket_list[bucket_index]->local_depth;
            bucket* b = bucket_list[bucket_index];
            string response = bucket_list[bucket_index]->insert(a);
            if( response == "success"){
                return "success";
            } else {
                double_directory();
                bucket_list[bucket_index]->local_depth = ld+1;
                for(int i=0;i<(1<<global_depth);i++){
                    if(i!= bucket_index && (i%(1<<ld))==(bucket_index%(1<<ld))){
                        cout<<"Local depth used for condition here is"<<ld<<endl;
                        cout<<"index for which new buckets were created: "<<i<<endl;
                        bucket_list[i] = new bucket(this->bucket_size, i, ld+1);
                    }
                }
                bucket_list[a%(1<<global_depth)]->list.push_back(a);
                for(int j=0;j<bucket_list[bucket_index]->list.size();j++){
                    if(bucket_list[bucket_index]->list[j]%(1<<(ld+1)) != bucket_index%(1<<ld+1)){
                        cout<<"value being re positioned is: "<<bucket_list[bucket_index]->list[j]<<endl;
                        bucket_list[bucket_list[bucket_index]->list[j]%(1<<global_depth)]->list.push_back(bucket_list[bucket_index]->list[j]);
                        // print_vector(bucket_list[bucket_list[bucket_index]->list[j]%(1<<global_depth)]->list);
                        bucket_list[bucket_index]->list.erase(bucket_list[bucket_index]->list.begin()+j);
                        j--;
                    }
                }
                return "success";
            }
        }

        string del(int a){
            string response = bucket_list[a%(1<<global_depth)]->del(a);
            return response;
        }

        string search(int a){
            int bucket_ind = a%(1<<global_depth);
            bucket* b = bucket_list[bucket_ind];
            for(int i=0;i<b->list.size();i++){
                if(b->list[i]==a){
                    string response = "bucket: "+to_string(bucket_ind)+" position: "+to_string(i);
                    return response;
                }
            }
            return "not found";
        }

        void print_status(){
            cout<<"Global Depth: "<< global_depth<<endl;
            set<bucket*>s;
            vector<bucket*>temp;
            for(int i=0;i<bucket_list.size();i++){
                temp.push_back(bucket_list[i]);
            }
            sort(temp.begin(), temp.end(),compare);
            for(int i=0;i<temp.size();i++){
                s.insert(temp[i]);
            }
            cout<<"Number of buckets: "<<s.size()<<endl;
            for(auto x : s){
                // cout<<x<<endl;
                bucket* b = x;
                cout<<"Bucket Details: "<<endl;
                cout<<b->list.size()<<" "<<b->local_depth<<"time created: "<<b->time_created<<endl;
                // cout<<"Bucket Data: "<<endl;
                // for(auto val:b->list){
                //     cout<<val<<" ";
                // }
                cout<<endl;
            }
        }
};

int main(){
    int g_d;
    int b_s;
    cin>>g_d>>b_s;
    hash_map* h = new hash_map(g_d, b_s);
    int choice, input;
    do{
        cin>>choice;
        if(choice == 2){
            cin>>input;
            // cout<<input<<endl;
            string response = h->insert(input);
            cout<<response<<endl;
        } else if(choice == 3){
            cin>>input;
            string response = h->search(input);
            cout<<response<<endl;
        } else if(choice == 4){
            cin>>input;
            string response = h->del(input);
            cout<<response<<endl;
        } else if(choice == 5){
            h->print_status();
        }

    }while(choice!=6);
    cout<<"Process quitted"<<endl;
    return 0;
}