#include "pch.h"
#include "SyntaxAnalyzer.h"

int FST_TRACE_n = -1;

TTM::MfstState::MfstState()
	: m_tape_position(0), m_nrule(-1), m_nrulechain(-1)
{	}

TTM::MfstState::MfstState(short position, MFSTSTACK m_stack, short m_nrulechain)
	: m_tape_position(position), m_nrule(-1), m_nrulechain(m_nrulechain), m_stack(m_stack)
{	}

TTM::MfstState::MfstState(short position, MFSTSTACK m_stack, short m_nrule, short m_nrulechain)
	: m_tape_position(position), m_nrule(m_nrule), m_nrulechain(m_nrulechain), m_stack(m_stack)
{	}

TTM::SyntaxAnalyzer::MfstDiagnosis::MfstDiagnosis()
	: m_tape_position(-1), rc_step(RC_STEP::SURPRISE), m_nrule(-1), nrule_chain(-1)
{	}

TTM::SyntaxAnalyzer::MfstDiagnosis::MfstDiagnosis(short m_tape_position, RC_STEP rc_step, short m_nrule, short nrule_chain)
	: m_tape_position(m_tape_position), rc_step(rc_step), m_nrule(m_nrule), nrule_chain(nrule_chain)
{	}

TTM::SyntaxAnalyzer::SyntaxAnalyzer()
	: m_tape(nullptr), m_tape_position(0), m_nrule(-1), m_nrulechain(-1), m_tape_size(0), lextable({}), greibach({})
{	}

TTM::SyntaxAnalyzer::SyntaxAnalyzer(const TTM::LexTable& lextable, const GRB::Greibach& greibach)
	: m_tape_position(0), m_nrule(-1), m_nrulechain(-1), m_tape_size(lextable.size()), lextable(lextable), greibach(greibach)
{
	m_tape = DBG_NEW short[m_tape_size];

	for (int k = 0; k < m_tape_size; ++k) {
		m_tape[k] = GRB::TS(lextable[k].lexeme);
	}

	m_stack.push(greibach.stbottomT);
	m_stack.push(greibach.startN);
}

std::string TTM::SyntaxAnalyzer::getCSt() {
	std::string output = "";

	for (int k = (signed)m_stack.size() - 1; k >= 0; --k) {
		short p = m_stack.c[k];
		output.push_back(GRB::Rule::Chain::alphabet_to_char(p));
	}

	return output;
}

std::string TTM::SyntaxAnalyzer::getCTape(short pos, short n) {
	std::string output = "";
	short i;
	short k = (pos + n < m_tape_size) ? pos + n : m_tape_size;

	for (i = pos; i < k; ++i) {
		output.push_back(GRB::Rule::Chain::alphabet_to_char(m_tape[i]));
	}

	return output;
}

std::string TTM::SyntaxAnalyzer::getDiagnosis(short n) {
	std::string output = "";
	std::stringstream ss;
	int errid = 0;
	int lpos = -1;

	if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].m_tape_position) >= 0) {
		errid = greibach.getRule(diagnosis[n].m_nrule).iderror;
		Error::ERROR err = Error::getErrorByCode(errid);
		ss << err.id << ": строка " << lextable[lpos].lineNumber << ", " << err.message;
		output = ss.str();
	}

	return output;
}

bool TTM::SyntaxAnalyzer::save_state() {
	m_storestate.push(MfstState(m_tape_position, m_stack, m_nrule, m_nrulechain));
	MFST_TRACE6("SAVESTATE:", m_storestate.size());
	return true;
}

bool TTM::SyntaxAnalyzer::restore_state() {
	bool output = false;
	MfstState state;

	if (output = (m_storestate.size() > 0)) {
		state = m_storestate.top();
		m_tape_position = state.m_tape_position;
		m_stack = state.m_stack;
		m_nrule = state.m_nrule;
		m_nrulechain = state.m_nrulechain;
		m_storestate.pop();
		MFST_TRACE5("RESTORESTATE")
			MFST_TRACE2
	}

	return output;
}

bool TTM::SyntaxAnalyzer::push_chain(GRB::Rule::Chain chain) {
	for (int k = chain.size - 1; k >= 0; k--) {
		m_stack.push(chain.nt[k]);
	}

	return true;
}

TTM::SyntaxAnalyzer::RC_STEP TTM::SyntaxAnalyzer::step() {
	RC_STEP output = SyntaxAnalyzer::RC_STEP::SURPRISE;
	if (m_tape_position < m_tape_size) {
		if (GRB::Rule::Chain::isN(m_stack.top())) {
			GRB::Rule rule;
			if ((m_nrule = greibach.getRule(m_stack.top(), rule)) >= 0) {
				GRB::Rule::Chain chain;
				if ((m_nrulechain = rule.getNextChain(m_tape[m_tape_position], chain, m_nrulechain + 1)) >= 0) {
					MFST_TRACE1
						save_state();
					m_stack.pop();
					push_chain(chain);
					output = SyntaxAnalyzer::RC_STEP::NS_OK;
					MFST_TRACE2
				}
				else {
					MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE")
						savediagnosis(SyntaxAnalyzer::RC_STEP::NS_NORULECHAIN);
					output = restore_state() ? SyntaxAnalyzer::RC_STEP::NS_NORULECHAIN : SyntaxAnalyzer::RC_STEP::NS_NORULE;
				};
			}
			else output = SyntaxAnalyzer::RC_STEP::NS_ERROR;
		}
		else if ((m_stack.top() == m_tape[m_tape_position])) {
			m_tape_position++;
			m_stack.pop();
			m_nrulechain = -1;
			output = SyntaxAnalyzer::RC_STEP::TS_OK;
			MFST_TRACE3
		}
		else {
			MFST_TRACE4("TS_NOK/NS_NORULECHAIN")
				output = restore_state()
				? SyntaxAnalyzer::RC_STEP::TS_NOK : SyntaxAnalyzer::RC_STEP::NS_NORULECHAIN;
		}
	}
	else {
		output = SyntaxAnalyzer::RC_STEP::TAPE_END;
		MFST_TRACE4("TAPE_END")
	};
	return output;
}

bool TTM::SyntaxAnalyzer::Start(Logger& log) {
	MFST_TRACE_START;

	bool output = false;
	RC_STEP rc_step = RC_STEP::SURPRISE;

	do {
		rc_step = step();
	} while (rc_step == SyntaxAnalyzer::RC_STEP::NS_OK || rc_step == SyntaxAnalyzer::RC_STEP::NS_NORULECHAIN
		|| rc_step == SyntaxAnalyzer::RC_STEP::TS_OK || rc_step == SyntaxAnalyzer::RC_STEP::TS_NOK);

	switch (rc_step) {
	case SyntaxAnalyzer::RC_STEP::TAPE_END:
		MFST_TRACE4("------>TAPE_END");
		log << "Синтаксический анализ выполнен без ошибок\n";
		output = true;
		break;

	case SyntaxAnalyzer::RC_STEP::NS_NORULE:
		MFST_TRACE4("------>NS_NORULE");
		log << getDiagnosis(0) << '\n';
		break;

	case SyntaxAnalyzer::RC_STEP::NS_NORULECHAIN:
		MFST_TRACE4("------>NS_NORULECHAIN");
		break;

	case SyntaxAnalyzer::RC_STEP::NS_ERROR:
		MFST_TRACE4("------>NS_ERROR");
		break;

	case SyntaxAnalyzer::RC_STEP::SURPRISE:
		MFST_TRACE4("------>SURPRISE");
		break;
	}

	return output;
}

bool TTM::SyntaxAnalyzer::savediagnosis(RC_STEP rc_step) {
	bool output = false;
	short k = 0;

	while (k < MFST_DIAGN_NUMBER && m_tape_position <= diagnosis[k].m_tape_position)
		k++;

	if (output = (k < MFST_DIAGN_NUMBER)) {
		diagnosis[k] = MfstDiagnosis(m_tape_position, rc_step, m_nrule, m_nrulechain);
		for (short j = k + 1; j < MFST_DIAGN_NUMBER; ++j) {
			diagnosis[j].m_tape_position = -1;
		}
	}

	return output;
}

void TTM::SyntaxAnalyzer::printrules() {
	MfstState state;
	GRB::Rule rule;
	for (unsigned short k = 0; k < m_storestate.size(); k++)
	{
		state = m_storestate.c[k];
		rule = greibach.getRule(state.m_nrule);
		MFST_TRACE7
	}
}

const std::string TTM::SyntaxAnalyzer::dumpTrace() const
{
	return m_trace.str();
}