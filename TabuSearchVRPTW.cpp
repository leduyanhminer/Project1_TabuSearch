#include <bits/stdc++.h>
using namespace std;

struct Customer {
  int id;
  double demand;
  double a, b;
};
vector<Customer>customers;
int m, n;
double cost[105][105],x[105],y[105], vehicle_capacity, best=INT_MAX;
bool check;
vector<vector<int>> routes, tabu_list;
vector<int>current_solution, route_demand, best_sol;
// các tham s? d?u vào
const int tabu_tenure = 5;
const int max_iterations = 50000;
const int num_neighbors = 200;
//
vector<int> create_greedy_initial_solution(){
  // tham lam l?i gi?i ban d?u
  vector<int> solution;
  vector<bool> visited(customers.size(),false);
  int cur_customer = 0;
  solution.push_back(cur_customer);

  while(solution.size() <= n){
    int nearest_customer = -1;
    int min_dis = INT_MAX;
    for(int i=1; i <= n; ++i){
        if(!visited[i]){
          if(cost[cur_customer][i] < min_dis){
            min_dis = cost[cur_customer][i];
            nearest_customer = i;
          }
        }
    }
    visited[nearest_customer] = true; //tham v? khách g?n nh?t
    cur_customer = nearest_customer;
    solution.push_back(cur_customer);
  }
  return solution;
}

double evaluate_solution(){
  // hàm dánh giá solution
  double total_dis = 0; //t?ng chi phí
  double total_penalty = 0; // t?ng ph?t
  routes.clear();
  route_demand.clear();
  vector<int>route;
  double cur_dem = 0; //tr?ng t?i hi?n t?i
  double cur_time = 0; //th?i gian hi?n t?i
  // luu các routes cho l?i gi?i hi?n t?i (current solution)
  for(int i=1; i<=n; ++i){
    int cus=current_solution[i];
    // n?u nh?n khách 'cus' vào mà vu?t quá tr?ng t?i t?i da
    // thì ta s? luu route vào và t?o route m?i
    if(cur_dem + customers[cus].demand > vehicle_capacity){
      total_dis+=cost[route[route.size()-1]][0];
      routes.push_back(route);
      route_demand.push_back(cur_dem);
      route.clear();
      cur_dem = 0;
    }
    route.push_back(cus); //nh?n khách hàng 'cus' vào route
    cur_dem += customers[cus].demand;
    if(route.size()==1){ // n?u dây là route m?i
      cur_time = max(cost[0][cus],customers[cus].a);
      total_dis += cost[0][cus]; // chi phí di chuy?n
    }
    else{ // route cu
      cur_time += cost[route[route.size()-2]][cus];
      total_dis += cost[route[route.size()-2]][cus]; // chi phí di chuy?n
    }
    cur_time = max(cur_time, customers[cus].a); // xét tru?ng h?p ph?i d?i d? dón khách
    if(cur_time > customers[cus].b){ //n?u th?i gian hi?n t?i vu?t quá th?i gian có th? ch? c?a khách
      total_penalty += (cur_time - customers[cus].b);
    }
  }
  if(route.size()>0){
    routes.push_back(route);
    route_demand.push_back(cur_dem);
    total_dis+=cost[route[route.size()-1]][0];
  }
  int x = routes.size();
  total_penalty+=max(x-m,0)*10000; // n?u solution s? d?ng quá m xe thì s? b? ph?t
  if(!total_penalty) check=true; //ki?m tra xem solution có b? vi ph?m time window không
  return total_dis + total_penalty;
  //cout<<total_dis<<" "<<total_penalty;
}

void generate_neighbor(){
// sinh hàng xóm b?ng cách d?i ch? 2 khách i j b?t k?
  int i = rand() % n + 1;
  int j = rand() % n + 1;
  while(j==i) j = rand() % n + 1;
  swap(current_solution[i], current_solution[j]);
}
int main(){
  // input
  cin >> m >> n >> vehicle_capacity;
  //customers.push_back({0,0,0,0});
  for(int i=0; i<=n; ++i){
    int demand, a, b;
    cin >> x[i] >> y[i] >> demand >> a >> b;
    customers.push_back({i,demand,a,b});
  }
  for(int i=0; i<=n; ++i)
    for(int j=0; j<=n; ++j){
        cost[i][j] = sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]));
    }
       // th?i gian di t? khách i->j
  // t?o solution ban d?u = phuong pháp tham lam
  current_solution = create_greedy_initial_solution();
  //cout<<evaluate_solution()<<'\n';
  tabu_list.resize(n+1);
  // tabu search
  int best_obj = evaluate_solution();
  int best_feasible_obj = INT_MAX;
  for(int i=0; i<max_iterations; ++i){
    vector<int>best_neighbor = current_solution;
    vector<int>best_feasible_neighbor;
    //sinh num_neighbors hàng xóm
    for(int j=1; j<=num_neighbors; ++j){
      generate_neighbor(); // sinh hàng xóm
      vector<int>neighbor = current_solution;
      bool is_tabu = false;
      for(int k=1; k<=n; ++k){
        // ki?m tra t?ng ph?n t? th? k xem có thu?c danh sách tabu_list[k] hay không
        if(find(tabu_list[k].begin(),tabu_list[k].end(), neighbor[k])!=tabu_list[k].end()){
          is_tabu = true;
          break;
        }
      }
      if(!is_tabu){
        check=false;
        int obj = evaluate_solution(); // tính giá tr? c?a solution
        if(check==true){
          if(obj<best) best=obj, best_sol=neighbor; //n?u penalty = 0 và t?t hon best thì c?p nh?t best
          if(obj<best_feasible_obj){
            best_feasible_obj = obj;   // tìm hàng xóm t?t nh?t (penalty = 0)
            best_feasible_neighbor = neighbor;
          }
        }
        if(obj < best_obj){
          best_neighbor = neighbor; // tìm hàng xóm t?t nh?t (penalty có th? !=0)
          best_obj = obj;
        }
      }
    }
    if(best_feasible_neighbor.size()){ // n?u t?n t?i hàng xóm có penalty = 0 thì l?y hàng xóm dó
      best_neighbor = best_feasible_neighbor;
      cout << 1 <<'\n';
    }
    for(int j=1; j<=n; ++j){
      tabu_list[j].push_back(best_neighbor[j]);
      if(tabu_list[j].size()>tabu_tenure){
        tabu_list[j].erase(tabu_list[j].begin());
      }
    }
    current_solution = best_neighbor;
  }
  // in ra final solution
    current_solution=best_sol;
    //if(best_sol.size()==0){
      //cout<<"NO SOLUTION!";
      //return 0;
    //}
    cout << "Final solution: ";
    for (int i = 1; i <= n; i++) {
        cout << current_solution[i] << " ";
    }
    cout << '\n';
    cout << "Total distance: " << evaluate_solution() << '\n';
  // in ra các routes
    cout << "Routes: " << '\n';
    for (int r = 0; r < routes.size(); r++) {
        cout << "Route " << r+1 << ": ";
        for (int c = 0; c < routes[r].size(); c++) {
            cout << routes[r][c] << " ";
        }
        cout << " (Demand: " << route_demand[r] << ")" << '\n';
    }
  return 0;
}
