/*
  Simple simulator in which states try to maximize their population.
  Each state will slowly grow every turn. Each turn, the state will have an option of either looking for a weaker opponent to invade, or to work on internal infrastructure.
  Based on the success of its strategy, each state will tweak its strategy accordingly.
 */

#include <stdio.h> // Desired for output.
#include <stdlib.h> // Desired for Random function
#include <unistd.h> // Desired for sleep function

/*
  STRUCTURE STATE:
  Past five population numbers (Queue would be great).
  Past five aggressiveness numbers.
  Aggressiveness
  Population
  Fertility

  PROCESS:
  Choose Random Action { Warmonger (search for a weaker state, invade), Infrastructure }
  Experience (or not) a Random Event { Disease (Random, percent loss of population. Between 1-10%), Secession (constant small percentage, creates new state with some random percentage of origial state's population and fertility. Aggressiveness is conserved.), Bounty (Random, 1-10% increase in fertility). }
  Turn (Population is increased based on some function of fertility. Aggressiveness is tweaked based on past experience.)
 */

#define MAXSTATES 100
#define MEMSIZE 5
#define MAXNAMESIZE 15
#define EMPTY 0 // If states[i] is EMPTY , it's considered empty.
#define EMPTYARRAY -1 // If a mem has an array value of EMPTYARRAY, it is considered empty there.

enum {TRUE = 1, FALSE = 0};

// CONSTANTS
static const double POWERRATIO = 1.25; // The population difference, expressed as a ratio, between an attacker and a defender that's sufficient to induce an attack.
static const double POPLOSS = .6; // The percentage population of the defending country that is destroyed. Removed from both sides.
static const double FERTTAKEN  = .2; // Fertility taken by the aggressor
static const double FERTDESTROYED = .4; // Fertility simply lost by the defender.
static const double IMPROVEEFFICIENCY = 1.10; // Fertility is increased by this multiple when self-improvement actions are taken.
static const double DISEASECHANCE = .03;
static const double SECESSIONCHANCE = .02;
static const double BOUNTYCHANCE = .03;
static const double STRATEGYMODIFIER = .5; // A modifier showing how quickly or slowly a state changes its aggressiveness.

// FUNCTIONS
struct pMem createP();
struct aMem createA();
struct state createState(char *name, long int pop, double ag, double fer);

// Utility functions
void randStr(char *dest, int length); // Basically stolen from Seb, Stack Overflow, due to my own disinterest in writing this out by hand at 4 in the morning.
void strCpy(char *dest, char *from); // Copy from into dest, while maintaining dest's size.

// Functions mediating between states.
void showAll(); // Print a brief description of all states.
void iterateAll(); // Iterate over all states.
int indexOfWeaker(struct state state); // Find a state sufficiently weaker than the current state.
void war(struct state *attacker, struct state *defender);
void removeDeadStates(); // All states with pop <= 0 are removed.
void addState(long int pop, double ag, double fer);

// Functions related to the state
void iterate(struct state *state);
void action(struct state *state);
void randomEv(struct state *state);
void turn(struct state *state);

// Functions related to the Mem structures.
int size(struct pMem mem);
void addPMem(long int num, struct pMem *mem);
int indexOfMax(struct pMem mem);

void addAMem(double num, struct aMem *mem);
double get(int index, struct aMem mem);

// STRUCTURES
struct pMem {
  int index;
  long int nums[MEMSIZE];
};

struct aMem {
  int index;
  double nums[MEMSIZE];
};

struct state {
  char name[MAXNAMESIZE];
  struct pMem popNums;
  struct aMem aggNums;
  long int population;
  double aggressiveness;
  double fertility;
};

// STATIC DATA STRUCTURES.
static struct state states[MAXSTATES];

// FUNCTION DEFINITIONS
int main(int argc, char *argv[]) {
  srand(time(NULL));
  int i;
  for(i = 0; i < MAXSTATES; i++)
    states[i] = createState("DEAD", 0, 0, 0);

  long int days = 1;
  
  long int mil = 1000000;
  double agInit = .5;
  double fertInit = 1.1;

  addState(mil, agInit, fertInit);
  addState(mil, agInit, fertInit);
  addState(mil, agInit, fertInit);
  //addState(mil, agInit, fertInit);
  //addState(mil, agInit, fertInit);
  
  while(TRUE) {
    printf("DAY %ld:\n", days);
    printf("______________________________\n");
    showAll();

    printf("\n");
    iterateAll();
    // removeDeadStates(); DEPRECATED SINCE SWITCH OVER TO NON-POINTER STATES[] ARRAY.
    printf("\n");
    days++;

    sleep(6);
  }

  return 0;
}

/*
  UTILITY FUNCTIONS
 */

void randStr(char *dest, int length) {
  char charset[] = "abcdefghijlmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  
  while (length-- > 0) {
    int index = (double) rand() / RAND_MAX * (sizeof charset - 1);
    *dest++ = charset[index];
  }
  *dest = '\0';
}

// Copy from into dest, while maintaining dest's size.
// ASSUMES FROM HAS SIZE <= DEST.
void strCpy(char *dest, char *from) {
  while(*from != '\0')
    *dest++ = *from++;
  while(*dest != '\0')
    *dest++ = ' ';
}

/*
  DEFINITIONS FOR FUNCTIONS THAT MEDIATE BETWEEN STATES
 */

// Print a brief description of all states.
void showAll() {
  int i;
  for(i = 0; i < MAXSTATES; i++) {
    if(states[i].population > EMPTY){
      struct state temp = states[i];
      printf("%s -\t Pop: %ld\t Aggr: %f\t Fert: %f\n", temp.name, temp.population, temp.aggressiveness, temp.fertility);
    }
  }
}

// Iterate over all states.
void iterateAll() {
  int i;
  for(i = 0; i < MAXSTATES; i++) {
    if(states[i].population > EMPTY)
      iterate(&(states[i]));
  }
}

// Find a state weaker than the parameter state.
int indexOfWeaker(struct state state) {
  int i;
  for(i = 0; i < MAXSTATES; i++) {
    if(states[i].population > EMPTY) {
      double ratio = (double) state.population / (states[i].population);
      if(ratio >= POWERRATIO) {
	return i;
      }
    } 
  }
  return -1;
}

// War. What is it good for?
void war(struct state *attacker, struct state *defender) {
  long int popLoss = (*defender).population * POPLOSS;
  double fertTaken = (*defender).fertility * FERTTAKEN;
  double fertDestroyed = (*defender).fertility * FERTDESTROYED;

  printf("%s INVADES %s. %ld KILLED, %f TOTAL PROPERTY DAMAGE\n", (*attacker).name, (*defender).name, popLoss, fertTaken + fertDestroyed);
  
  (*attacker).population -= popLoss;
  (*attacker).fertility += fertTaken;
  
  (*defender).population -= popLoss;
  (*defender).fertility -= (fertTaken + fertDestroyed);
}

// Remove dead states. Called after all states have done their turn.
// DEPRECATED FUNCTION WHEN STATES[] CONTAINED POINTERS.
/*
void removeDeadStates() {
  int i = 0;
  for(i = 0; i < MAXSTATES; i++) {
    if(states[i] != EMPTY) {
      if((*states[i]).population <= 0) {
	states[i] = EMPTY;
      }
    }
  }
}
*/

// Add state to states[]
void addState(long int pop, double ag, double fer) {
  int i;
  for(i = 0; i < MAXSTATES; i++) {
    if(states[i].population <= EMPTY) {
      char name[MAXNAMESIZE];
      // int rand = random() % MAXNAMESIZE + (MAXNAMESIZE/3);
      // DEPRECATED. ALL NAMES WILL HAVE EQUAL SIZES.
      randStr(name, MAXNAMESIZE);

      struct state temp = createState(name, pop, ag, fer);
      states[i] = temp;
      break;
    }
  }
}

/*
  DEFINITIONS FOR INTRA-STATE FUNCTIONS.
 */
struct state createState(char *name, long int pop, double ag, double f) {
  struct state temp;
  
  int i;
  strCpy(&(temp.name), name); // Was useful in maintaining formatting while having variable length names.
  temp.popNums = createP();
  temp.aggNums = createA();
  temp.population = pop;
  temp.aggressiveness = ag;
  temp.fertility = f;
  return temp;
}

void iterate(struct state *state) {
  action(state);
  randomEv(state);
  turn(state);
}

void action(struct state *state) {
  int aggressiveness = (*state).aggressiveness * 100;
  int randomChoice = random() % 100;
  
  if(aggressiveness >= randomChoice) { // Trigger war action.
    int weakerStateIndex = indexOfWeaker(*state);
    if(weakerStateIndex >= 0)
      war(state, &(states[weakerStateIndex]));
  } else { // Infrastructure Improvement
    (*state).fertility = (*state).fertility * IMPROVEEFFICIENCY;
  }
}

void randomEv(struct state *state) {
  long int rand = random() % 100;
  if(rand <= 100 * DISEASECHANCE) {
    double loss = (double) ((random() % 10) + 1) / 100;

    printf("DISEASE STRIKES %s: %f%% OF POPULATION TRAGICALLY DEAD\n", (*state).name, loss * 100);
    (*state).population -= (long int) (*state).population * loss;
    return;
  }
  
  rand = random() % 100;
  if(rand <= 100 * SECESSIONCHANCE) {
    double loss = (double) (random() % 100) / 100;
    
    long int lostPop = (long int) (*state).population * loss;
    double lostFertility = (*state).fertility * loss;

    printf("CIVIL WAR STRIKES %s: DEFECTORS ARE ESTIMATED AT %ld, TAKING %f WORTH OF ASSETS WITH THEM\n", (*state).name, lostPop, lostFertility);

    addState(lostPop, (*state).aggressiveness, lostFertility);
    (*state).population -= lostPop;
    (*state).fertility -= lostFertility;

    return;
  }

  rand = random() % 100;
  if(rand <= 100 * BOUNTYCHANCE) {
    double gain = ((random() % 10) + 1) / 100;

    printf("NEW TECHNOLOGY IMPROVES FERTILITY IN %s: GAINS MEASURED AT %f\n", (*state).name, gain);

    (*state).fertility *= (1 + gain);
    return;
  }
}

void turn(struct state *state) {
  long int pop = (*state).population;
  double fertility = (*state).fertility;
  
  (*state).population = pop * fertility;

  // Reconfigure aggressiveness based on past success.
  if(size((*state).popNums) == MEMSIZE) {
    int index = indexOfMax((*state).popNums);
    int direction = ((*state).population - ((*state).popNums).nums[index] >= 0) ? 1 : -1; // Continue trend or discontinue trend.
    double aggDif = (*state).aggressiveness - ((*state).aggNums).nums[index];
    printf("AGG DIFF FOR %s is %f\n", state->name, aggDif);
    double newAggressiveness = (*state).aggressiveness + (direction * aggDif * STRATEGYMODIFIER);

    // Make sure aggressiveness is within bounds.    
    if(newAggressiveness < 0) {
      newAggressiveness = 0.0;
    } else if (newAggressiveness > 1) {
      newAggressiveness = 1.0;
    }

    (*state).aggressiveness = newAggressiveness;
  }

  addPMem((*state).population, &((*state).popNums));
  addAMem((*state).aggressiveness, &((*state).aggNums));
}

/*
  DEFINITIONS FOR PMEM FUNCTIONS
 */
struct pMem createP() {
  struct pMem temp;
  temp.index = 0;
  
  int i;
  for(i = 0; i < MEMSIZE; i++)
   temp.nums[i] = EMPTYARRAY;

  return temp;
}

void addPMem(long int num, struct pMem *mem) {
  if((*mem).index == MEMSIZE - 1)
    (*mem).index = 0;
  (*mem).nums[(*mem).index] = num;
  ((*mem).index)++;
}
// Return the size of a pMem structure. In practice, we're only interested
// in how many elements of the nums array have been filled.
int size(struct pMem mem) {
  int n, sz;
  
  for(n = 0, sz = 0; n <= MEMSIZE; n++) {
    if(mem.nums[n] != EMPTYARRAY)
      sz++;
  }
  return sz;
}

// Return the max value of a pMem.
int indexOfMax(struct pMem mem) {
  int n, index, max;
  
  for(n = 0, index = 0, max = -9999999; n <= MEMSIZE; n++) {
    if(mem.nums[n] >= max)
      index = n;
  }
  return index;
}

/*
  DEFINITIONS FOR AMEM FUNCTIONS
 */
struct aMem createA() {
  struct aMem temp;
  temp.index = 0;

  int i;
  for(i = 0; i < MEMSIZE; i++)
    temp.nums[i] = EMPTYARRAY;

  return temp;
}

void addAMem(double num, struct aMem *mem) {
  if((*mem).index == MEMSIZE - 1)
    (*mem).index = 0;
  (*mem).nums[(*mem).index] = num;
  ((*mem).index)++;
}

double get(int index, struct aMem mem) {
  return mem.nums[index];
}
