#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <array>
#include <functional>
#include <string>

#define ALL_POSSIBLE_OPTIONS 3
#define POSSIBLE_OPTIONS ALL_POSSIBLE_OPTIONS-1
using Gene = std::vector<char>;
char mapIntegrer(int i);
int randInt(const int min, const int max);


Gene generateGene(){
    Gene toReturn;
    for(auto & g: toReturn){
        g =  mapIntegrer(randInt(0,ALL_POSSIBLE_OPTIONS-1));
    }
    return toReturn;
}

auto print = [](const Gene & g){
    std::cout<<"[ ";
    for(auto const & k: g){
        std::cout<<k<<", ";
    }
    std::cout<<"]\n";
};



class SchemeResolver{
private:
    static std::vector<std::pair<bool,Gene>>* allSchemas;

public:
    static void generateAllSchemas(){
        if(allSchemas == nullptr)
            allSchemas = new std::vector<std::pair<bool,Gene>>();    
        Gene g;
        std::vector<Gene> temp_vec;
        auto map_func = [](Gene& g, const unsigned level, unsigned i){ g[level-1] = mapIntegrer(i);};
        generateSchemasOfLevel(temp_vec, g, map_func);
        for(auto const & gene: temp_vec){
            allSchemas->push_back(std::make_pair(false, gene));
        }
    }    

    static void generateSchemasOfLevel( std::vector<Gene> & vec_ref, 
                                        Gene prev_g, 
                                        std::function<void(Gene&, unsigned, unsigned)> map_func,                                        
                                        unsigned possible_options = ALL_POSSIBLE_OPTIONS, 
                                        unsigned level = 0)
    {
        if(level < GENE_SIZE)
            level++;
            for(unsigned i = 0; i < possible_options; i++){
                map_func(prev_g, level, i);
                if(level == GENE_SIZE){
                    vec_ref.push_back(prev_g);
                }else{
                    generateSchemasOfLevel(vec_ref, prev_g, map_func,possible_options, level);
                }
            }  
        return;     
    }

    static void deleteAllSchemas(){
        delete allSchemas;
    }

    static void clearSchemaMarks(){
        for(auto & i: *allSchemas){
            i.first = false;
        }
    }

    static void printAllSchemas(){
        for(auto & i: *allSchemas){
            print(i.second);
        }
    }
    

    static bool checkSchema(Gene & g){
        for(auto & s: *allSchemas){
            if(s.first == true) continue;
            if(s.second == g) {
                s.first = true;
                return true;
            }
        }
        return false;
    }

    static unsigned checkGeneration(const std::vector<Gene> & generation){
        unsigned counter = 0;
        clearSchemaMarks();
        for(auto const & gene: generation ){
            auto schemas = generateSchemasOfLevels(gene);
            for(auto & schema: schemas){
                if(checkSchema(schema)) counter++;
            }
        }
        return counter;
    }

    static std::vector<Gene> generateSchemasOfLevels(const Gene& g){
        auto map_func = [](Gene& g, const unsigned level, unsigned i){ 
            g[level-1] =  i == 0 ? g[level-1]  : '#';
        };

        std::vector<Gene> toReturn;
        generateSchemasOfLevel(toReturn, g, map_func, POSSIBLE_OPTIONS);
        return toReturn;
    }

};

std::vector<std::pair<bool,Gene>> * SchemeResolver::allSchemas;

std::vector<Gene> generatePopulation(unsigned sizeOfPopulation){
    std::vector<Gene> toReturn;
    for(unsigned i = 0; i < sizeOfPopulation; i++){
       toReturn.push_back(generateGene());
    }
    return toReturn;
}

unsigned tenTimesRun(int i){
    auto generation = generatePopulation(i);
    unsigned sum = 0;
    for(int u = 0; u < 10; u++){
        sum += SchemeResolver::checkGeneration(generation);        
    }
    return static_cast<unsigned>(sum/10);
}

void testCase(const unsigned GENE_SIZE){
    SchemeResolver::generateAllSchemas();
    std::fstream fs;
    std::string filename = "test_genSize_" + std::to_string(GENE_SIZE) + ".dat";
    fs.open(filename, std::fstream::out);
    for(int i = 1; i <= 100; i++){
        auto num = tenTimesRun(i);
        fs << i << "\t" << num << std::endl;
    }
    for(int i = 100; i <= 300; i+=10){
        auto num = tenTimesRun(i);
        fs << i << "\t" << num << std::endl;
    }
    for(int i = 300; i <= 500; i+=50){
        auto num = tenTimesRun(i);
        fs << i << "\t" << num << std::endl;
    }
    for(int i = 500; i <= 1000; i+=100){
        auto num = tenTimesRun(i);
        fs << i << "\t" << num << std::endl;
    }
    fs.close();
    SchemeResolver::deleteAllSchemas();
}

std::vector<unsigned> prepareTestData(){
    std::vector<unsigned> testData = {
        6// 6,7,8,9,10,11,12,13,14,15,16,17
    };
    return testData;
}

void prodRun(){
    auto testData = prepareTestData();    
    for(auto const & genSize: testData){
        testCase(genSize);
    }
}

void testRun(unsigned i){
    SchemeResolver::generateAllSchemas();
    std::fstream fs;
    fs.open("data8.dat", std::fstream::out);
    auto generation = generatePopulation(i);
    std::cout << generation.size() << '\n';
    for(auto& a: generation){
        print(a);
    }
    auto num = SchemeResolver::checkGeneration(generation);    
    fs << i << "\t" << num << std::endl;
    fs.close();
    SchemeResolver::deleteAllSchemas();
}

int main(int argc, char**argv){ 
    // testRun(2);
    prodRun();
}

char mapIntegrer(int i){
    switch (i){
        case 0:
            return '0';
            break;
        case 1:
            return '1';
            break;
        case 2:
            return '#';
            break;
        default:
            return '#';
            break;
    }
}

int randInt(const int min, const int max){
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(min, max - 1);
    return dis(gen);
}