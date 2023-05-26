#include "fmt/core.h"

#include "enums.hh"
#include "fc_iengine.hh"
#include "fmt/format.h"

namespace ie {
bool FCIEngine::solve() {
	if (!parse())
		throw std::invalid_argument("Wrong format for FC");

	return entailment_check();
}

bool FCIEngine::parse() {
	std::sregex_token_iterator it(m_kb_str.begin(), m_kb_str.end(), HORN_REGEX);
	std::sregex_token_iterator end;

	int mode = 0;
	std::vector<std::string> literals;
	for (std::string token, prev = ""; it != end; ++it) {
		token = *it;
		switch (token[0]) {
		case '&':
			continue;
		case '=':
			++mode;
			break;
		case ';':
			if ((literals.size() + mode) == 1)
				m_inferred_facts.insert(literals[0]);
			literals.clear();
			mode = 0;
			break;
		default:
			if (mode)
				m_kb[token] = literals;
			else
				literals.push_back(token);
			break;
		}
	}
	fmt::println("");

	return true;
}

bool FCIEngine::entailment_check() {
	bool new_fact_inferred = false;
	for (auto &[right_side, left_side] : m_kb) {
		// if leftside is cleared, skip this sentence
		if (left_side.empty())
			continue;

		fmt::println("Right: {}", right_side);
		fmt::println("Left: {}", fmt::join(left_side, ","));
		fmt::println("++++++++++++++++++++++++");
	}
	while (true) {
		// is the m_query_str already a fact?
		/* fmt::println("------------------------------"); */
		/* for (const auto &val : m_inferred_facts) { */
		/* 	fmt::print("{}, ", val); */
		/* } */
		/* fmt::println("\n{}", m_query_str); */

		if (m_inferred_facts.find(m_query_str) != m_inferred_facts.end())
			return true;

		// checking each fact in the fact queue
		for (const auto &cur_fact : m_inferred_facts) {
			// check each sentence in m_kb
			for (auto &[right_side, left_side] : m_kb) {
				// if leftside is cleared, skip this sentence
				if (left_side.empty())
					continue;

				// find current fact symbol in the left side
				auto it = std::find(left_side.begin(), left_side.end(), cur_fact);
				// if found, erase from left side to keep track of undetermined symbols
				if (it != left_side.end())
					left_side.erase(it);

				// if leftside is cleared, add right side to fact queue
				if (left_side.size() > 0) {
					continue;
				}

				// only add new inferred fact if it is actually new
				// (not already in list of inferred facts)
				new_fact_inferred = (m_inferred_facts.find(right_side) == m_inferred_facts.end());
				if (new_fact_inferred) {
					m_inferred_facts.insert(right_side);
					break;
				}
			}
			if (new_fact_inferred)
				break;
		}
		if (!new_fact_inferred)
			break;
		new_fact_inferred = false;
	}

	return false;
}

void FCIEngine::result_output() {
	for (const auto &element : m_inferred_facts) {
		fmt::print("{} ", element);
	}
	fmt::println("");
}
} // namespace ie