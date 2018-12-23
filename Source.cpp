#include <iostream>
#include <ctime>
#include <tuple>
#include <vector>
#include <algorithm>

class Individual {

  int fitness;
  std::vector<int> genes;

public:
  
  Individual::Individual() : fitness(0), genes(10, 0) { // !hard code!
    srand(time(nullptr));

    for (auto i = 0; i < genes.size(); ++i) {
      genes[i] = rand() % 2;
    }
  }

  int Individual::calculate_fitness() {
    fitness = 0;

      for (auto i = 0; i < genes.size(); ++i) {
        fitness += genes[i];
      }

      return fitness;
  }

  friend void crossover(Individual& fittest, Individual& vice_fittest);
  friend void mutation(Individual& fittest, Individual& vice_fittest);
  friend void add_fittest_offspring(Individual& fittest, Individual& vice_fittest);
};

class Population {
  int population_size;
  std::vector<Individual> individuals;

public:

  Population::Population(const int population_size) : population_size(population_size) {
    for (auto i = 0; i < population_size; ++i) {
      individuals.push_back(Individual());
    }
  }
  
  std::tuple<Individual, Individual> Population::get_fittest_individuals() {
    int the_fittest, the_vice_fittest;
  
    if (individuals[0].calculate_fitness() >= individuals[1].calculate_fitness()) {
      the_fittest = 0, the_vice_fittest = 1;  
    }
    else {
      the_fittest = 1, the_vice_fittest = 0;
    }

    auto max_fitness = std::max(individuals[0].calculate_fitness(), individuals[1].calculate_fitness());
    auto vice_max_fitness = std::min(individuals[0].calculate_fitness(), individuals[1].calculate_fitness());

    for (auto i = 2; i < individuals.size(); ++i) {
      const auto current_fitness = individuals[i].calculate_fitness();

      if (current_fitness > max_fitness) {
        vice_max_fitness = max_fitness;
        the_vice_fittest = the_fittest;
        max_fitness = current_fitness;
        the_fittest = i;
      }
      else if (current_fitness <= max_fitness && current_fitness > vice_max_fitness) {
        vice_max_fitness = current_fitness;
        the_vice_fittest = i;
      }
    }

    return std::make_tuple(individuals[the_fittest], individuals[the_vice_fittest]);
  }

  void Population::calculate_individuals_fitness() {
    for (auto i = 0; i < population_size; ++i) {
      individuals[i].calculate_fitness();
    }
  }

  int Population::get_least_fit_individual_index() {
    auto garbage_index = 0;
    auto garbage_fitness = individuals[0].calculate_fitness();

    for (auto i = 1; i < individuals.size(); ++i) {
      if (individuals[i].calculate_fitness() < garbage_fitness) {
        garbage_fitness = individuals[i].calculate_fitness();
        garbage_index = i;
      }
    }

    return garbage_index;
  }

  friend void add_fittest_offspring(Individual& fittest, Individual& vice_fittest);
};

Population initial_population(1000);
Individual fittest, vice_fittest;
auto generation_count = 0;

void selection(Individual& fittest, Individual& vice_fittest) {
  std::tie(fittest, vice_fittest) = initial_population.get_fittest_individuals();
}

void crossover(Individual& fittest, Individual& vice_fittest) {
  srand(time(nullptr));

  const int crossover_point = rand() % fittest.genes.size();

  for (auto i = 0; i < crossover_point; ++i) {
    std::swap(fittest.genes[i], vice_fittest.genes[i]);
  }
}

void mutation(Individual& fittest, Individual& vice_fittest) {
  srand(time(nullptr));

  int mutation_point = rand() % fittest.genes.size();

  fittest.genes[mutation_point] = 1 - fittest.genes[mutation_point];

  mutation_point = rand() % fittest.genes.size();

  vice_fittest.genes[mutation_point] = 1 - vice_fittest.genes[mutation_point];
}

void add_fittest_offspring(Individual& fittest, Individual& vice_fittest) {
  fittest.calculate_fitness();
  vice_fittest.calculate_fitness();

  if (fittest.fitness > vice_fittest.fitness) {
    initial_population.individuals[initial_population.get_least_fit_individual_index()] = fittest;
  }
  else {
    initial_population.individuals[initial_population.get_least_fit_individual_index()] = vice_fittest;
  }
}

int main() {
  srand(time(nullptr));

  initial_population.calculate_individuals_fitness();

  fittest = std::get<0>(initial_population.get_fittest_individuals());

  std::cout << "Generation: " << generation_count << " Fittest: " << fittest.calculate_fitness() << '\n';

  while(fittest.calculate_fitness() < 10) {
    generation_count++;

    selection(fittest, vice_fittest);
    crossover(fittest, vice_fittest);

    if (rand() % 12 < 10) {
      mutation(fittest, vice_fittest);
    }

    add_fittest_offspring(fittest, vice_fittest);

    std::cout << "Generation: " << generation_count << " Fittest: " << fittest.calculate_fitness() << '\n';
  }
  return 0;
}
