#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <ctime>
#include <utility>
#include <stack>
#include <cmath>

struct MadeReader{
    std::ifstream reader;
    int _len;
};

struct FlightNumber{
    std::string airport;
    int id;
};

struct DepartureTime
{
    int hh;
    int mm;
};

struct Record{
    FlightNumber flight_number;
    DepartureTime time;
    int cost;
    std::vector<std::string> departure_days;
};

struct ToReturnVec{
    std::vector<Record> data;
    long long int perm_count;
    long long int comp_count;
};

struct ToReturnArr{
    Record* data;
    long long int perm_count;
    long long int comp_count;
};

struct ToReturnQ{
    long long int ind;
    long long int perm_count;
    long long int comp_count;
};

template< typename T >
std::vector<T> reversed(std::vector<T> data){
    std::vector<T> to_return(data);
    std::reverse(to_return.begin(), to_return.end());
    return to_return;
}

template< typename T>
void print(std::vector<T> data, bool reversed = false){
    if (reversed == false){
        for (T elem: data){
            std::cout<< elem;
        }
    }else{
        std::reverse(data.begin(), data.end());
        for (T elem: data){
            std::cout<< elem ; //<< std::endl;
        }
    }
    std::cout<< std::endl;
}

template<typename T>
std::vector<T> slice(std::vector<T> data, int first_ind, int second_ind){
    if (second_ind > lenght(data)){
        std::cout<<"Second ind error";
        return data;
    }
    if (first_ind < 0){
        std::cout<<"First ind error";
    }
    std::vector<T> new_data;
    for(int ind= first_ind; ind<second_ind; ind++){
        new_data.push_back(data[ind]);
    }
    return new_data;
}

void print_data_around_ind(Record data[], int ind){
    std::cout<<ind<<std::endl;
    for (int i  = ind - 4; i < ind + 4; i ++){
        std::cout<<data[i].cost<<std::endl;
    }
}


std::string slice(std::string data, int first_ind, int second_ind){
    std::string result="";
    if (second_ind > data.size()){
        std::cout<<"Second ind error";
        return data;
    }
    if (first_ind < 0){
        std::cout<<"First ind error";
    }
    for (int ind = first_ind; ind < second_ind; ind ++){
        result += data[ind];
    }
    return result;
}

std::vector<std::string> split(std::string line, char delim){
    std::vector<std::string> to_return;
    std::stringstream ss(line);
    std::string part_line;
    while (std::getline(ss, part_line, delim)){
        to_return.push_back(part_line);
    }
    return to_return;
}

std::string make_string_from_Record(Record data){
    std::string result;
    result += data.flight_number.airport;
    result += std::to_string(data.flight_number.id) + " ";
    result += std::to_string(data.time.hh) +":"\
    + std::to_string(data.time.mm) + " ";
    result += std::to_string(data.cost) + " ";
    for (std::string elem: data.departure_days){
        result += elem + " ";
    }
    return result;
}

void print_arr(Record* data, int _len){
    for (int i = 0; i < _len; i++){
        std::cout<<make_string_from_Record(data[i]) << std::endl;
    }
}

void print_vec(std::vector<Record>data){
    for(Record elem: data){
        std::cout<<make_string_from_Record(elem)<<std::endl;
    }
}

bool is_sorted_arr(Record* data, int _len){
    for (int i =1; i < _len; i++){
        if (data[i].cost < data[i-1].cost){
            return false;
        }
    }
    return true;
}

bool is_sorted_vec(std::vector<Record> data){
    for (int i =1; i< data.size(); i ++){
        if (data[i].cost  < data[i-1].cost){
            return false;
        }
    }
    return true;
}

MadeReader make_reader(std::string root_to_input_file){
    MadeReader to_return;
    std::ifstream reader(root_to_input_file);
    std::string line_n;
    int n=0;
    if (std::getline(reader, line_n)){
        n = std::stoi(line_n);
    }else{
        throw std::invalid_argument( "Wrong file!" );
    }
    to_return.reader = std::move(reader);
    to_return._len = n;
    return to_return;
}

Record make_record(std::string line){
    Record to_return;
    auto to_rec = split(line, ',');
    FlightNumber now_FN;
    DepartureTime now_DP;
    now_FN.airport = slice(to_rec[0], 0, 2);
    now_FN.id = std::stoi(slice(to_rec[0], 2, 5));
    auto to_DP = split(to_rec[1], ':');
    now_DP.hh = std::stoi(to_DP[0]);
    now_DP.mm = std::stoi(to_DP[1]);
    to_return.flight_number = now_FN;
    to_return.time = now_DP;
    to_return.cost = std::stoi(to_rec[2]);
    if (to_rec.size() == 4){
        to_return.departure_days  = split(to_rec[3], ' ');
    }else{
        to_return.departure_days = {};
    }
    return to_return;
}

ToReturnArr insert_sort_arr(Record* data, int _len){
    Record* new_data = new Record[_len];
    long long int comp_count = 0;
    long long int perm_count = 0;
    int count = 0;
    for(int i= 0; i < _len; i++){
        int ind  = -1;
        for(int j =0; j < count; j ++){
            if(new_data[j].cost > data[i].cost){
                ind= j;
                break;
            }
            comp_count++;
        }
        if (ind != -1){
            for(int j=count-1; j >= ind; j--){
                new_data[j +1] = new_data[j];
                perm_count ++;
            }
            new_data[ind] = data[i];
            perm_count++;
            count++;
        }else {
            new_data[count] = data[i];
            count++;
        }
        comp_count++;
    }
    ToReturnArr to_return;
    to_return.data = new_data;
    to_return.perm_count = perm_count;
    to_return.comp_count = comp_count;
    return to_return;
}

Record* shell_sort_arr(Record* data, int _len){
    long long int comp_count = 0;
    long long int perm_count = 0;
    for (int i=_len/2; i >= 1; i /= 2){
        std::vector<int> indeces;
        for (int j =0; j <i; j++){
            indeces.push_back(j);
            int count = 1;
            while (j + count * i <= _len-1){
                indeces.push_back(j + count * i);
                count++;
            }
            Record* to_sort = new Record[count];
            for (int k= 0; k < count; k ++){
                perm_count++;
                to_sort[k] = data[indeces[k]];
            }
            auto from_insert = insert_sort_arr(to_sort, count);
            to_sort = from_insert.data;
            perm_count += from_insert.perm_count;
            comp_count += from_insert.comp_count;
//            print_arr(to_sort, count);
            for (int k= 0; k < count; k ++){
                 data[indeces[k]] = to_sort[k];
                 perm_count++;
            }
            indeces.clear();
        }
    }
    std::cout<<"Count of compression: "<<comp_count<<std::endl;
    std::cout<<"Count of permutations: "<<perm_count<<std::endl;
    return data;
}

ToReturnVec insert_sort_vec(std::vector<Record> data){
    std::vector<Record> new_data;
    long long int comp_count = 0;
    long long int perm_count = 0;
    for (int i = 0; i < data.size(); i++){
        new_data.push_back(data[i]);
        int ind = -1;
        for(int j = 0; j < new_data.size(); j ++) {
            if (data[i].cost < new_data[j].cost){
                ind = j;
                break;
            }
            comp_count ++;
        }
        if (ind != -1){
            for(int j = new_data.size()-1; j > ind; j--){
                Record to_swap = new_data[j-1];
                new_data[j-1] = new_data[j];
                new_data[j] = to_swap;
                perm_count ++;
            }
        }
        comp_count++;
    }
    ToReturnVec to_return;
    to_return.data = new_data;
    to_return.comp_count = comp_count;
    to_return.perm_count = perm_count;
    return to_return;
}

std::vector<Record> shell_sort_vec(std::vector<Record> data){
    long long int comp_count = 0;
    long long int perm_count = 0;
    for (int i = data.size() / 2; i >= 1; i /=2){
        std::vector<int> indices;
        for (int j =0; j <i; j++) {
            indices.push_back(j);
            int count = 1;
            while (j + count * i <= data.size() - 1) {
                indices.push_back(j + count * i);
                count++;
            }
            std::vector<Record> to_sort;
            for (int k = 0; k < count; k++) {
                to_sort.push_back(data[indices[k]]);
            }
            ToReturnVec returned = insert_sort_vec(to_sort);
            to_sort = returned.data;
            comp_count += returned.comp_count;
            perm_count += returned.perm_count;
            for (int k = 0; k < count; k++) {
                data[indices[k]] = to_sort[k];
                perm_count++;
            }
            indices.clear();
        }
    }
    std::cout<<"Count of compression: "<<comp_count<<std::endl;
    std::cout<<"Count of permutations: "<<perm_count<<std::endl;
    return data;
}

ToReturnQ partition_arr(Record*& data, int left, int right){
    long long int comp_count = 0;
    long long int perm_count = 0;
    Record start = data[(left + right) / 2];
    int i = left;
    int j = right;
    while(i<= j){
        while (i <= right && data[i].cost <= start.cost) {
            i++;
        }
        while (j >= left  && data[j].cost > start.cost) {
            j--;
        }
        if (i >= j) {
            break;
        }
        comp_count++;
        Record to_swap = data[i];
        data[i] = data[j];
        data[j] = to_swap;
        perm_count++;
    }
    ToReturnQ to_return;
    to_return.ind = j;
    to_return.perm_count = perm_count;
    to_return.comp_count = comp_count;

    return to_return;
}

void qsort_arr(Record*& data, int left, int right){
    long long int comp_count = 0;
    long long int perm_count = 0;
    std::stack<std::pair<int, int>> moves;
    moves.push({left, right});
    while(moves.empty() == false){
        std::pair<int, int> left_right = moves.top();
        moves.pop();
        if (left_right.second<= left_right.first) {
            continue;
        }
        comp_count ++;
        ToReturnQ new_data = partition_arr(data, left_right.first, left_right.second);
        long long int i = new_data.ind;
        perm_count += new_data.perm_count;
        comp_count += new_data.comp_count;
        if(i - left_right.first > left_right.second - i){
            moves.push({left_right.first, i - 1});
            moves.push({i+1, left_right.second});
        }else{
            moves.push({i+1, left_right.second});
            moves.push({left_right.first, i - 1});
        }
        comp_count ++;
    }
    std::cout<<"Count of compression: "<<comp_count<<std::endl;
    std::cout<<"Count of permutations: "<<perm_count<<std::endl;
}

ToReturnQ new_partition_vec(std::vector<Record>& data, int left, int right){
    long long int comp_count = 0;
    long long int perm_count = 0;
    Record start = data[(left + right) / 2];
    int i = left;
    int j = right;
    while(i<= j){
        while (i <= right && data[i].cost <= start.cost) {
            i++;
        }
        while (j >= left  && data[j].cost > start.cost) {
            j--;
        }
        if (i >= j) {
            break;
        }
        comp_count++;
        Record to_swap = data[i];
        data[i] = data[j];
        data[j] = to_swap;
        perm_count++;
    }
    ToReturnQ to_return;
    to_return.ind = j;
    to_return.perm_count = perm_count;
    to_return.comp_count = comp_count;
    return to_return;
}

void new_qsort_vec(std::vector<Record>& data, int left, int right){
    long long int comp_count = 0;
    long long int perm_count = 0;
    std::stack<std::pair<int, int>> moves;
    moves.push({left, right});
    while(moves.empty() == false){
        std::pair<int, int> left_right = moves.top();
        moves.pop();
        if (left_right.second<= left_right.first) {
            continue;
        }
        comp_count ++;
        ToReturnQ new_data = new_partition_vec(data, left_right.first, left_right.second);
        long long int i = new_data.ind;
        perm_count += new_data.perm_count;
        comp_count += new_data.comp_count;
        if(i - left_right.first > left_right.second - i){
            moves.push({left_right.first, i - 1});
            moves.push({i+1, left_right.second});
        }else{
            moves.push({i+1, left_right.second});
            moves.push({left_right.first, i - 1});
        }
        comp_count ++;
    }
    std::cout<<"Count of compression: "<<comp_count<<std::endl;
    std::cout<<"Count of permutations: "<<perm_count<<std::endl;
}

int main(){
    std::string root_to_input_file;
    std::cout<<"Enter file name(start from disk)"<<std::endl;
    //C:\Users\Alexandr\CLionProjects\Lab_work3\little.txt
    std::cin>>root_to_input_file;
    std::string line_n;
    MadeReader reader = make_reader(root_to_input_file);
    Record* raw_arr = new Record[reader._len];
    std::string line ="";
    int ind=0;
    auto start_time = clock();
    while (std::getline(reader.reader, line)){
        Record to_add = make_record(line);
        raw_arr[ind] = to_add;
        ind++;
    }
    auto end_time = clock();
    std::cout<<"Time of add to dinamyc arr: "<<end_time - start_time<<std::endl;
//    print_arr(raw_arr, reader._len);
    std::vector<Record> data;
    reader = make_reader(root_to_input_file);
    start_time = clock();
    while (std::getline(reader.reader, line)){
        Record to_add = make_record(line);
        data.push_back(make_record(line));
    }
    end_time = clock();
    std::cout<<"Time of add to vector: "<<end_time - start_time<<std::endl;
    start_time = clock();
    auto sorted_arr = shell_sort_arr(raw_arr, reader._len);
    end_time = clock();
    std::cout<<"Time to shell_sort for array: " << end_time - start_time<<std::endl;
    std::cout<<is_sorted_arr(sorted_arr, reader._len)<<std::endl;//work wrong!
//    print_arr(sorted_arr, reader._len);
    std::cout<<"------------------------------"<<std::endl;
    start_time = clock();
    auto sorted_vec = shell_sort_vec(data);
    end_time = clock();
    std::cout<<"Time to shell_sort for vector: " << end_time - start_time<<std::endl;
    std::cout<< is_sorted_vec(sorted_vec)<<std::endl;
//    std::cout<< is_sorted_vec(data)<<std::endl;
//    print_vec(sorted_vec);
    std::cout<<"------------------------------"<<std::endl;
    reader = make_reader(root_to_input_file);
    Record* raw_data = new Record[reader._len];
    ind  = 0;
    while (std::getline(reader.reader, line)){
        Record to_add = make_record(line);
        raw_data[ind] = to_add;
        ind++;
    }
    start_time = clock();
    qsort_arr(raw_data,0 ,reader._len-1);
    end_time = clock();
    std::cout<<"Time to qsort for array: " << end_time - start_time<<std::endl;
    std::cout<<is_sorted_arr(raw_arr, reader._len)<<std::endl;//work wrong!
//    print_arr(raw_arr, reader._len);
    std::cout<<"------------------------------"<<std::endl;
    start_time = clock();
//    sorted_vec = qsort_vec(data,0 ,data.size()-1);
    new_qsort_vec(data, 0, data.size() - 1);
    end_time = clock();
    std::cout<<"Time to qsort for vec: " << end_time - start_time<<std::endl;
    std::cout<<is_sorted_vec(sorted_vec)<<std::endl;//work wrong!
//    print_vec(sorted_vec);
// C:\Users\Alexandr\CLionProjects\Lab_work3\new_output.txt
    return 0;
}