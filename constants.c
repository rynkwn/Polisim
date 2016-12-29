#include "constants.h"

// CONSTANTS
const double POWERRATIO = 2.00; // The population difference, expressed as a ratio, between an attacker and a defender that's sufficient to induce an attack.
const double POPLOSS = .6; // The percentage population of the defending country that is destroyed. Removed from both sides.
const double FERTTAKEN  = .2; // Fertility taken by the aggressor
const double FERTDESTROYED = .4; // Fertility simply lost by the defender.
const double IMPROVEEFFICIENCY = 1.10; // Fertility is increased by this multiple when self-improvement actions are taken.
const double DISEASECHANCE = .03;
const double SECESSIONCHANCE = .02;
const double BOUNTYCHANCE = .03;
const double STRATEGYMODIFIER = .5; // A modifier showing how quickly or slowly a state changes its aggressiveness.

const int OPENNESS_TO_STATREGY_EXPERIMENTATION = 4; // Higher numbers lead to greater absolute value in strategy changes. If too high, no consistent strategy. If too low, experiments slowly. If 0, no aggression change.
const double MINIMUM_STRATEGY_CHANGE = .1; // Minimum strategy change.

const long int POP_SUPPORTED_BY_ONE_FERTILITY = 2000000; // The amount of population supported by a single unit of fertility. If current pop is less than this, will increase. Otherwise, will either be stable or decrease.

char *COUNTRY_NAMES[70] = {
	"Achaemenid Empire",
	"Adena Culture",
	"Ammon",
	"Anga",
	"Athens",
	"Avanti",
	"Bithynia",
	"Cappadocia",
	"Carthaginian Empire",
	"Chavín Culture",
	"Chedi",
	"Chen",
	"Chera Kingdom",
	"Chola",
	"Chorrera",
	"Chu",
	"Colchis",
	"Corinth",
	"Cyrene",
	"D'mt",
	"Dardanian Kingdom",
	"Edom",
	"Gandhara",
	"Gojoseon",
	"Han",
	"Japan",
	"Kalinga",
	"Kamboja",
	"Kasi",
	"Kikata",
	"Kosala",
	"Kuru",
	"Kush",
	"Lu",
	"Lycaonia",
	"Macedonia",
	"Mahajanapadas",
	"Malla",
	"Mannai",
	"Maya Civilization",
	"Minaea",
	"Moab",
	"Mysia",
	"Nanda Empire",
	"Olmec",
	"Panchala",
	"Pandya",
	"Paphlagonia",
	"Paracas Culture",
	"Pisidia",
	"Qi",
	"Qin",
	"Roman Republic",
	"Saba",
	"Scythia",
	"Sparta",
	"Surasena",
	"Ta Netjeru",
	"Thebes",
	"Vajji",
	"Van Lang",
	"Vatsa",
	"Wei",
	"Yan",
	"Zapotec Civilization",
	"Zheng",
	"Zhou"
};

