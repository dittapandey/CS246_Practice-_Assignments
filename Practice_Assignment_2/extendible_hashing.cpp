#include<bits/stdc++.h>
using namespace std;

// struct bucket{
//     int local_depth;
//     vector<int>bucket;
// };

// struct hash_map{
//     int global_depth;
//     vector<bucket*>list_bucket;
// };

class bucket{
    public:
        int hash_index;
        int local_depth;
        int size;
        vector<int>list;
        bucket(int s, int h_ind, int l_d){
            hash_index=h_ind;
            local_depth=l_d;
            size=s;
        }

        string insert(int a){
            if(this->list.size()==size){
                return "bucket full";
            } else {
                list.push_back(a);
                return "success";
            }
        }
};

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
            }
        }

        void double_directory(){
            for(int i=0;i<(1<<global_depth);i++){
                bucket_list.push_back(bucket_list[i]);
            }
            global_depth++;
        }

        string insert(int a){
            int bucket_index = a%(1<<(this->global_depth));
            bucket* b = bucket_list[bucket_index];
            if(bucket_list[bucket_index]->insert(a) == "success"){
                return "success";
            } else {
                double_directory();
                for(int i=0;i<(1<<global_depth);i++){
                    if(bucket_list[i]->list.size() == bucket_list[i]->size){
                        bucket_list[i]->local_depth+=1;
                        bucket_list[i+(1<<global_depth)] = new bucket(this->bucket_size, i+(1<<global_depth), bucket_list[i]->local_depth);
                        int ld = bucket_list[i]->local_depth;
                        for(auto val: bucket_list[i]->list){
                            bucket_list
                            
                        }
                    }
                }
            }
        }
};

int main(){


}