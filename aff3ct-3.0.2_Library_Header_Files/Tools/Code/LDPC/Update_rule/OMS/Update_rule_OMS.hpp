/*!
 * \file
 * \brief Class tools::Update_rule_OMS.
 */
#ifndef UPDATE_RULE_OMS_HPP
#define UPDATE_RULE_OMS_HPP

#include <string>

#include "Tools/Code/LDPC/Update_rule/MS/Update_rule_MS.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Update_rule_OMS // Offset Min Sum
{
protected:
	const std::string name;
	const R offset;
	Update_rule_MS<R> MS;

public:
	explicit Update_rule_OMS(const R offset);

	virtual ~Update_rule_OMS() = default;

	inline std::string get_name() const;

	inline void begin_decoding(const int n_ite);

	inline void begin_ite(const int ite);

	// incoming values from the variable nodes into the check nodes
	inline void begin_chk_node_in(const int chk_id, const int chk_degree);

	inline void compute_chk_node_in(const int var_id, const R var_val);

	inline void end_chk_node_in();

	// outcomming values from the check nodes into the variable nodes
	inline void begin_chk_node_out(const int chk_id, const int chk_degree);

	inline R compute_chk_node_out(const int var_id, const R var_val);

	inline void end_chk_node_out();

	inline void end_ite();

	inline void end_decoding();
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/LDPC/Update_rule/OMS/Update_rule_OMS.hxx"
#endif

#endif /* UPDATE_RULE_OMS_HPP */