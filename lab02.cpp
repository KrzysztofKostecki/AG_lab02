#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <array>

#define ALL_POSSIBLE_OPTIONS 3
#define POSSIBLE_OPTIONS ALL_POSSIBLE_OPTIONS-1
#define GENE_SIZE 6
using Gene = std::array<char, GENE_SIZE>;
char mapIntegrer(int i);
int randInt(const int min, const int max);


Gene generateGene(){
    Gene toReturn;
    for(auto & g: toReturn){
        g =  mapIntegrer(randInt(0,POSSIBLE_OPTIONS-1));
    }
    return toReturn;
}

auto print = [](Gene & g){
    std::cout<<"[ ";
    for(auto & k: g){
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
        Gene g ;
        std::vector<Gene> temp_vec;

        generateSchemasOfLevel(temp_vec, g);
        for(auto const & gene: temp_vec){
            allSchemas->push_back(std::make_pair(false, gene));
        }
    }    

    static void generateSchemasOfLevel(std::vector<Gene> & vec_ref, Gene prev_g, unsigned possible_options = ALL_POSSIBLE_OPTIONS, unsigned level = 0){
        if(level < GENE_SIZE)
            level++;
            for(int i = 0; i < possible_options; i++){
                prev_g[level-1] = mapIntegrer(i);
                if(level == GENE_SIZE){
                    vec_ref.push_back(prev_g);
                }else{
                    generateSchemasOfLevel(vec_ref, prev_g, possible_options, level);
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
        auto map = [](int i, char value) -> char{return i == 0 ? value : '#';};
        
        std::vector<Gene> toReturn;
        Gene g;
        generateSchemasOfLevel(toReturn, g, POSSIBLE_OPTIONS, map);
        for(int i1 = 0; i1 < 2; i1++)
            for(int i2 = 0; i2 < 2; i2++)
                for(int i3 = 0; i3 < 2; i3++)
                    for(int i4 = 0; i4 < 2; i4++)
                        for(int i5 = 0; i5 < 2; i5++)
                            for(int i6 = 0; i6 < 2; i6++){
                                    Gene toAdd = {map(i1, g[0]),map(i2, g[1]),map(i3, g[2]), map(i4,g[3]),map(i5, g[4]),map(i6, g[5])};
                                    toReturn.push_back(toAdd);
                            }
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

void prodRun(){
    SchemeResolver::generateAllSchemas();
    std::fstream fs;
    fs.open("data.dat", std::fstream::out);
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

void testRun(){
    SchemeResolver::generateAllSchemas();
    std::fstream fs;
    fs.open("data.dat", std::fstream::out);
    auto num = tenTimesRun(1000);
    fs << 1 << "\t" << num << std::endl;
    fs.close();
    SchemeResolver::deleteAllSchemas();
}

int main(int argc, char**argv){ 
    // testRun();
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