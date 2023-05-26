#ifndef CLAUSE_HH
#define CLAUSE_HH

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace ie {
struct Clause {
	std::string token;
	std::string op;
	std::shared_ptr<Clause> left  = nullptr;
	std::shared_ptr<Clause> right = nullptr;
	bool neg                      = false;

	static std::unordered_map<std::string, std::shared_ptr<Clause>> Clauses;
	static std::unordered_set<std::string> Literals;
	static std::unordered_map<std::string, int> TruthTable;
	static std::shared_ptr<Clause> Negation(std::shared_ptr<Clause> left);
	static std::shared_ptr<Clause> Disjunction(std::shared_ptr<Clause> left,
	                                           std::shared_ptr<Clause> right);
	static std::shared_ptr<Clause> Conjunction(std::shared_ptr<Clause> left,
	                                           std::shared_ptr<Clause> right);
};
}; // namespace ie

#endif // CLAUSE_HH
