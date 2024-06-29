#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <climits>
#include <deque>
#include <string>
#include <sstream>
#include <algorithm>
#include "Heap.h"




class Graph_M {
    struct Vertex {
        std::unordered_map<std::string, int> nbrs;
    };

    struct DijkstraPair {
        std::string vname;
        std::string psf;
        int cost;

        bool operator<(const DijkstraPair& other) const {
            return cost > other.cost; // min-heap based on cost
        }
    };

    struct Pair {
        std::string vname;
        std::string psf;
        int min_dis;
        int min_time;
    };

    std::unordered_map<std::string, Vertex> vtces;

public:
    void addVertex(const std::string& vname) {
        Vertex vtx;
        vtces[vname] = vtx;
    }

    void addEdge(const std::string& vname1, const std::string& vname2, int value) {
        if (vtces.find(vname1) == vtces.end() || vtces.find(vname2) == vtces.end()) {
            return;
        }
        vtces[vname1].nbrs[vname2] = value;
        vtces[vname2].nbrs[vname1] = value;
    }

    bool containsVertex(const std::string& vname) {
        return vtces.find(vname) != vtces.end();
    }

    bool containsEdge(const std::string& vname1, const std::string& vname2) {
        if (vtces.find(vname1) == vtces.end() || vtces.find(vname2) == vtces.end()) {
            return false;
        }
        return vtces[vname1].nbrs.find(vname2) != vtces[vname1].nbrs.end();
    }

    void display_Map() {
        std::cout << "\t Delhi Metro Map" << std::endl;
        std::cout << "\t------------------" << std::endl;
        std::cout << "----------------------------------------------------\n" << std::endl;

        for (const auto& pair : vtces) {
            std::string str = pair.first + " =>\n";
            for (const auto& nbr : pair.second.nbrs) {
                str += "\t" + nbr.first + "\t\t" + std::to_string(nbr.second) + "\n";
            }
            std::cout << str << std::endl;
        }
        std::cout << "\t------------------" << std::endl;
        std::cout << "---------------------------------------------------\n" << std::endl;
    }

    void display_Stations() {
        std::cout << "\n***********************************************************************\n" << std::endl;
        int i = 1;
        for (const auto& pair : vtces) {
            std::cout << i << ". " << pair.first << std::endl;
            ++i;
        }
        std::cout << "\n***********************************************************************\n" << std::endl;
    }

    bool hasPath(const std::string& vname1, const std::string& vname2, std::unordered_map<std::string, bool>& processed) {
        if (containsEdge(vname1, vname2)) {
            return true;
        }
        processed[vname1] = true;
        for (const auto& nbr : vtces[vname1].nbrs) {
            if (!processed[nbr.first]) {
                if (hasPath(nbr.first, vname2, processed)) {
                    return true;
                }
            }
        }
        return false;
    }

    int dijkstra(const std::string& src, const std::string& des, bool nan) {
        int val = 0;
        std::vector<std::string> ans;
        std::unordered_map<std::string, DijkstraPair> map;
        Heap <DijkstraPair> heap;

        for (const auto& pair : vtces) {
            DijkstraPair np;
            np.vname = pair.first;
            np.psf = "";
            np.cost = INT_MAX;
            if (pair.first == src) {
                np.cost = 0;
                np.psf = pair.first;
            }
            heap.add(np);
            map[pair.first] = np;
        }

        while (!heap.isEmpty()) {
            DijkstraPair rp = heap.remove();
            if (rp.vname == des) {
                val = rp.cost;
                break;
            }
            map.erase(rp.vname);
            ans.push_back(rp.vname);

            for (const auto& nbr : vtces[rp.vname].nbrs) {
                if (map.find(nbr.first) != map.end()) {
                    int oc = map[nbr.first].cost;
                    int nc = nan ? rp.cost + 120 + 40 * vtces[rp.vname].nbrs[nbr.first] : rp.cost + vtces[rp.vname].nbrs[nbr.first];

                    if (nc < oc) {
                        DijkstraPair& gp = map[nbr.first];
                        gp.psf = rp.psf + nbr.first;
                        gp.cost = nc;
                        heap.updatePriority(gp);
                    }
                }
            }
        }
        return val;
    }

    std::string Get_Minimum_Distance(const std::string& src, const std::string& dst) {
        int min = INT_MAX;
        std::string ans = "";
        std::unordered_map<std::string, bool> processed;
        std::deque<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;

        stack.push_front(sp);

        while (!stack.empty()) {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed.find(rp.vname) != processed.end()) {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst) {
                if (rp.min_dis < min) {
                    ans = rp.psf;
                    min = rp.min_dis;
                }
                continue;
            }

            for (const auto& nbr : vtces[rp.vname].nbrs) {
                if (processed.find(nbr.first) == processed.end()) {
                    Pair np;
                    np.vname = nbr.first;
                    np.psf = rp.psf + nbr.first + "  ";
                    np.min_dis = rp.min_dis + vtces[rp.vname].nbrs[nbr.first];
                    stack.push_front(np);
                }
            }
        }
        ans += std::to_string(min);
        return ans;
    }

    std::string Get_Minimum_Time(const std::string& src, const std::string& dst) {
        int min = INT_MAX;
        std::string ans = "";
        std::unordered_map<std::string, bool> processed;
        std::deque<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;

        stack.push_front(sp);

        while (!stack.empty()) {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed.find(rp.vname) != processed.end()) {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst) {
                if (rp.min_time < min) {
                    ans = rp.psf;
                    min = rp.min_time;
                }
                continue;
            }

            for (const auto& nbr : vtces[rp.vname].nbrs) {
                if (processed.find(nbr.first) == processed.end()) {
                    Pair np;
                    np.vname = nbr.first;
                    np.psf = rp.psf + nbr.first + "  ";
                    np.min_time = rp.min_time + 120 + 40 * vtces[rp.vname].nbrs[nbr.first];
                    stack.push_front(np);
                }
            }
        }
        ans += std::to_string(static_cast<int>(std::ceil(static_cast<double>(min) / 60)));
        return ans;
    }

    std::vector<std::string> get_Interchanges(const std::string& str) {
        std::vector<std::string> arr;
        std::istringstream iss(str);
        std::string station;
        while (std::getline(iss, station, ' ')) {
            arr.push_back(station);
        }
        return arr;
    }

    void Create_Metro_Map() {
        addVertex("Noida Sector 62~B");
        addVertex("Botanical Garden~B");
        addVertex("Yamuna Bank~B");
        addVertex("Rajiv Chowk~BY");
        addVertex("Vaishali~B");
        addVertex("Moti Nagar~B");
        addVertex("Janak Puri West~BO");
        addVertex("Dwarka Sector 21~B");
        addVertex("Huda City Center~Y");
        addVertex("Saket~Y");
        addVertex("Vishwavidyalaya~Y");
        addVertex("Chandni Chowk~Y");
        addVertex("New Delhi~YO");
        addVertex("AIIMS~Y");
        addVertex("Shivaji Stadium~O");
        addVertex("DDS Campus~O");
        addVertex("IGI Airport~O");
        addVertex("Rajouri Garden~BP");
        addVertex("Netaji Subhash Place~PR");
        addVertex("Punjabi Bagh West~P");

        addEdge("Noida Sector 62~B", "Botanical Garden~B", 8);
        addEdge("Botanical Garden~B", "Yamuna Bank~B", 10);
        addEdge("Yamuna Bank~B", "Vaishali~B", 8);
        addEdge("Yamuna Bank~B", "Rajiv Chowk~BY", 6);
        addEdge("Rajiv Chowk~BY", "Moti Nagar~B", 9);
        addEdge("Moti Nagar~B", "Janak Puri West~BO", 7);
        addEdge("Janak Puri West~BO", "Dwarka Sector 21~B", 6);
        addEdge("Huda City Center~Y", "Saket~Y", 15);
        addEdge("Saket~Y", "AIIMS~Y", 6);
        addEdge("AIIMS~Y", "Rajiv Chowk~BY", 7);
        addEdge("Rajiv Chowk~BY", "New Delhi~YO", 1);
        addEdge("New Delhi~YO", "Chandni Chowk~Y", 2);
        addEdge("Chandni Chowk~Y", "Vishwavidyalaya~Y", 5);
        addEdge("New Delhi~YO", "Shivaji Stadium~O", 2);
        addEdge("Shivaji Stadium~O", "DDS Campus~O", 7);
        addEdge("DDS Campus~O", "IGI Airport~O", 8);
        addEdge("Moti Nagar~B", "Rajouri Garden~BP", 2);
        addEdge("Punjabi Bagh West~P", "Rajouri Garden~BP", 2);
        addEdge("Punjabi Bagh West~P", "Netaji Subhash Place~PR", 3);
    }

    std::vector<std::string> printCodelist() {
        std::cout << "List of station along with their codes:\n" << std::endl;
        std::vector<std::string> keys;
        for (const auto& pair : vtces) {
            keys.push_back(pair.first);
        }

        std::vector<std::string> codes(keys.size());
        int i = 1, j = 0, m = 1;
        for (const auto& key : keys) {
            std::istringstream stname(key);
            std::string temp;
            char c;
            codes[i - 1] = "";
            while (stname >> temp) {
                c = temp[0];
                while (c >= '0' && c <= '9') {
                    codes[i - 1] += c;
                    j++;
                    c = temp[j];
                }
                if (c < '0' || c > '9') {
                    codes[i - 1] += c;
                }
            }
            if (codes[i - 1].length() < 2) {
                codes[i - 1] += std::toupper(temp[1]);
            }

            std::cout << i << ". " << key << "\t";
            if (key.length() < (22 - m)) std::cout << "\t";
            if (key.length() < (14 - m)) std::cout << "\t";
            if (key.length() < (6 - m)) std::cout << "\t";
            std::cout << codes[i - 1] << std::endl;
            i++;
            if (i == static_cast<int>(std::pow(10, m))) m++;
        }
        return codes;
    }

    void mainMenu() {
        Create_Metro_Map();

        std::cout << "\n\t\t\t****WELCOME TO THE METRO APP*****" << std::endl;
        std::string choice;

        while (true) {
            std::cout << "\t\t\t\t~~LIST OF ACTIONS~~\n\n";
            std::cout << "1. LIST ALL THE STATIONS IN THE MAP" << std::endl;
            std::cout << "2. SHOW THE METRO MAP" << std::endl;
            std::cout << "3. GET SHORTEST DISTANCE FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << std::endl;
            std::cout << "4. GET SHORTEST TIME TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << std::endl;
            std::cout << "5. GET SHORTEST PATH (DISTANCE WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << std::endl;
            std::cout << "6. GET SHORTEST PATH (TIME WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << std::endl;
            std::cout << "7. EXIT THE MENU" << std::endl;
            std::cout << "\nENTER YOUR CHOICE FROM THE ABOVE LIST (1 to 7) : ";
            std::getline(std::cin, choice);

            if (choice == "7") {
                exit(0);
            }

            std::cout << "\n***********************************************************\n";
            std::unordered_map<std::string, bool> processed;
            std::string st1, st2;
            if (choice == "1") {
                display_Stations();
            } else if (choice == "2") {
                display_Map();
            } else if (choice == "3") {
                std::vector<std::string> keys = printCodelist();
                std::cout << "\n1. TO ENTER SERIAL NO. OF STATIONS\n2. TO ENTER CODE OF STATIONS\n3. TO ENTER NAME OF STATIONS\n";
                std::cout << "ENTER YOUR CHOICE: ";
                std::string ch;
                std::getline(std::cin, ch);

                std::cout << "ENTER THE SOURCE AND DESTINATION STATIONS" << std::endl;
                if (ch == "1") {
                    std::string st1Index, st2Index;
                    std::getline(std::cin, st1Index);
                    std::getline(std::cin, st2Index);
                    st1 = keys[std::stoi(st1Index) - 1];
                    st2 = keys[std::stoi(st2Index) - 1];
                } else if (ch == "2") {
                    std::string a, b;
                    std::getline(std::cin, a);
                    std::getline(std::cin, b);
                    auto it = std::find(keys.begin(), keys.end(), a);
                    st1 = *it;
                    it = std::find(keys.begin(), keys.end(), b);
                    st2 = *it;
                } else if (ch == "3") {
                    std::getline(std::cin, st1);
                    std::getline(std::cin, st2);
                } else {
                    std::cout << "Invalid choice" << std::endl;
                    exit(0);
                }

                if (!containsVertex(st1) || !containsVertex(st2) || !hasPath(st1, st2, processed)) {
                   

                    std::cout << "THE INPUTS ARE INVALID" << std::endl;
                } else {
                    std::cout << "SHORTEST DISTANCE FROM " << st1 << " TO " << st2 << " IS " << dijkstra(st1, st2, false) << "KM\n";
                }
            } else if (choice == "4") {
                std::cout << "ENTER THE SOURCE STATION: ";
                std::getline(std::cin, st1);
                std::cout << "ENTER THE DESTINATION STATION: ";
                std::getline(std::cin, st2);

                if (!containsVertex(st1) || !containsVertex(st2) || !hasPath(st1, st2, processed)) {
                    std::cout << "THE INPUTS ARE INVALID" << std::endl;
                } else {
                    std::cout << "SHORTEST TIME FROM (" << st1 << ") TO (" << st2 << ") IS " << dijkstra(st1, st2, true) / 60 << " MINUTES\n\n";
                }
            } else if (choice == "5") {
                std::cout << "ENTER THE SOURCE AND DESTINATION STATIONS" << std::endl;
                std::getline(std::cin, st1);
                std::getline(std::cin, st2);

                if (!containsVertex(st1) || !containsVertex(st2) || !hasPath(st1, st2, processed)) {
                    std::cout << "THE INPUTS ARE INVALID" << std::endl;
                } else {
                    std::vector<std::string> str = get_Interchanges(Get_Minimum_Distance(st1, st2));
                    int len = str.size();
                    std::cout << "SOURCE STATION : " << st1 << std::endl;
                    std::cout << "SOURCE STATION : " << st2 << std::endl;
                    std::cout << "DISTANCE : " << str[len - 1] << std::endl;
                    std::cout << "NUMBER OF INTERCHANGES : " << str[len - 2] << std::endl;
                    std::cout << "~~~~~~~~~~~~~" << std::endl;
                    std::cout << "START  ==>  " << str[0];
                    for (int i = 1; i < len - 3; ++i) {
                        std::cout << " ==> " << str[i];
                    }
                    std::cout << " ==> END" << std::endl;
                    std::cout << "~~~~~~~~~~~~~" << std::endl;
                }
            } else if (choice == "6") {
                std::cout << "ENTER THE SOURCE STATION: ";
                std::getline(std::cin, st1);
                std::cout << "ENTER THE DESTINATION STATION: ";
                std::getline(std::cin, st2);

                if (!containsVertex(st1) || !containsVertex(st2) || !hasPath(st1, st2, processed)) {
                    std::cout << "THE INPUTS ARE INVALID" << std::endl;
                } else {
                    std::vector<std::string> str = get_Interchanges(Get_Minimum_Time(st1, st2));
                    int len = str.size();
                    std::cout << "SOURCE STATION : " << st1 << std::endl;
                    std::cout << "DESTINATION STATION : " << st2 << std::endl;
                    std::cout << "TIME : " << str[len - 1] << " MINUTES" << std::endl;
                    std::cout << "NUMBER OF INTERCHANGES : " << str[len - 2] << std::endl;
                    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
                    std::cout << "START  ==>  " << str[0];
                    for (int i = 1; i < len - 3; ++i) {
                        std::cout << " ==> " << str[i];
                    }
                    std::cout << " ==> END" << std::endl;
                    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
                }
            } else {
                std::cout << "Please enter a valid option! " << std::endl;
                std::cout << "The options you can choose are from 1 to 6. " << std::endl;
            }
        }
    }
};

int main() {
    Graph_M g;
    g.mainMenu();
    return 0;
}
