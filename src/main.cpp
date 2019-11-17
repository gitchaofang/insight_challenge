#include<fstream>
#include<string>
#include<iostream>
#include<vector>
#include<unordered_map>
#include<map>
#include<queue>
#include<iterator>

/*get info for each line*/
std::vector<std::string> info(std::string& str,std::unordered_map<uint32_t,uint32_t>& col){
    std::vector<std::string> res(4);
    uint32_t pos = 0;
    while(str != ""){
        std::string s("");
        if(str.find(',') == std::string::npos){
            s = str;
            str = "";
        }
        else{
            s = str.substr(0,str.find(','));
            str.erase(0,str.find(',') + 1);
        }

        if(s == "") return {};
        if(col.find(pos) == col.end()){
                ++pos;
                continue;
        } 
            
        if(col[pos] == 2){
            std::string year = s.substr(6,4);
            res[col[pos]] = (year + '/' + s);
        }
        else res[col[pos]] = s;
        ++pos;
    }
    std::string ts = res[0] + '|' + res[1] + '|' + res[2];
    return {ts,res[3]};
}

/*seperate info for printing*/
std::vector<std::string> sep(std::string &str){
    std::vector<std::string> res;
    while(true){
        if(str.find('|') == std::string::npos){
            res.push_back(str);
            str = "";
            break;
        }    
        uint32_t pos = str.find('|');
        res.push_back(str.substr(0,pos));
        str.erase(0,pos+1);
    }
    return res;
}


class cmp{
    public:
        bool operator()(const std::vector<std::string>& v1, const std::vector<std::string>& v2){
            if(v1[2] == v2[2]){
                if(v1[3] == v2[3]){
                    if(v1[1] == v2[1])return v1[0] < v2[0];
                    else return v1[1] < v2[1];
                }
                else return std::stoi(v1[3]) < std::stoi(v2[3]);
            }
            return v1[2] < v2[2];          
        }
};


int main(){
    /*read input file*/
    std::fstream fp;
    fp.open("./input/Border_Crossing_Entry_Data.csv",std::ios::in);
    std::string cache("");
    std::map<std::string, std::string>m;
    std::unordered_map<uint32_t,uint32_t> col;
    std::getline(fp,cache);
    uint32_t i = 0;
    std::unordered_map<std::string,uint32_t> position({{"Border",0},{"Date",2},{"Measure",1},{"Value",3}});
    while(!cache.empty()){
        std::string str("");
        if(cache.find(',') == std::string::npos){
            str = cache;
            cache = "";
        }
        else{
            uint32_t pos = cache.find(',');
            str = cache.substr(0,pos);
            cache.erase(0,pos+1);
        }
        if((str != "Date") && (str != "Border") && (str != "Measure") && (str != "Value")){
            ++i; 
            continue;
        }
        col.insert({i,position[str]});
        ++i;
    }
   
    std::getline(fp,cache);
    while(!cache.empty()){
        std::vector<std::string> tv = info(cache,col);
        if(tv.empty()) continue;
        if(m.find(tv[0]) == m.end()) m.insert({tv[0],tv[1]});
        else m[tv[0]] = std::to_string(std::stoi(m[tv[0]]) + std::stoi(tv[1]));
        std::getline(fp,cache);
    }
    fp.close();

    /*put data in order*/
    std::priority_queue<std::vector<std::string>, std::vector<std::vector<std::string>>,cmp> res;
    std::unordered_map<std::string, std::pair<int,int>>count;
    for (auto it = m.begin(); it != m.end(); ++it){
        std::string str = it -> first;
        std::string head = str.substr(0,str.find('/'));
        int ave = 0;
        if(count.find(head) == count.end()){
            ave = 0;
            count.insert({head,{1,std::stoi(it -> second)}});
        }
        else{
            double td = count[head].second;
            td = td / double(count[head].first) + 0.5; 
            ave = int(td);
            ++count[head].first;
            count[head].second += std::stoi(it -> second);
        }

        std::vector<std::string> v = sep(str); 
        v.push_back(it -> second);   
        v.push_back(std::to_string(ave));
        res.push(v);
    }
    
    /*printing*/
    fp.open("./output/report.csv",std::ios::out);
    fp << "Border,Date,Measure,Value,Average" << '\n';
    while(!res.empty()){
        std::vector<std::string> v = res.top();
        res.pop();
        v[2].erase(0,5);
        fp << v[0] << ','<<v[2] << ',' << v[1] << ',' << v[3] <<',' << v[4]<< '\n';
    }

    return 0;
}
