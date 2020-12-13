#pragma once
#include <stack>
#include "Greibach.h"
#include "LexTable.h"
#include "Error.h"
#include "Logger.h"

#pragma region TRACE

#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

#define MFST_TRACE_START m_trace << std::setw(4)<<std::left<<"Шаг"<<": " \
	<< std::setw(30) << std::left << "Правило"  \
	<< std::setw(30) << std::left << "Входная лента" \
	<< std::setw(20) << std::left << "Стек" \
	<< '\n';

#define MFST_TRACE1 m_trace <<std::setw(4)<<std::left<<++FST_TRACE_n<<": " \
	<< std::setw(30) << std::left << rule.getCRule(m_nrulechain)  \
	<< std::setw(30) << std::left << getCTape(m_tape_position) \
	<< std::setw(20) << std::left << getCSt() \
	<< '\n';

#define MFST_TRACE2    m_trace <<std::setw(4)<<std::left<<FST_TRACE_n<<": " \
	<< std::setw(30) << std::left << " "  \
	<< std::setw(30) << std::left << getCTape(m_tape_position) \
	<< std::setw(20) << std::left << getCSt() \
	<< '\n';

#define MFST_TRACE3     m_trace<<std::setw(4)<<std::left<<++FST_TRACE_n<<": " \
	<< std::setw(30) << std::left << " "  \
	<< std::setw(30) << std::left << getCTape(m_tape_position) \
	<< std::setw(20) << std::left << getCSt() \
	<< '\n';

#define MFST_TRACE4(c) m_trace<<std::setw(4)<<std::left<<++FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<'\n';
#define MFST_TRACE5(c) m_trace<<std::setw(4)<<std::left<<  FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<'\n';

#define MFST_TRACE6(c,k) m_trace<<std::setw(4)<<std::left<<++FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<k<<'\n';

#define MFST_TRACE7  m_trace<<std::setw(4)<<std::left<<state.m_tape_position<<": "\
	<< std::setw(20) << std::left << rule.getCRule(state.m_nrulechain) \
	<< '\n';

#pragma endregion

template<typename T>
struct use_container : T
{
	using T::T;
	using T::c;
};

typedef use_container<std::stack<short>> MFSTSTACK;

namespace TTM
{
	struct MfstState
	{
		short m_tape_position;
		short m_nrule;
		short m_nrulechain;
		MFSTSTACK m_stack;

		MfstState();
		MfstState(short position, MFSTSTACK m_stack, short m_nrulechain);
		MfstState(short position, MFSTSTACK m_stack, short m_nrule, short m_nrulechain);
	};

	class SyntaxAnalyzer
	{
	public:
		SyntaxAnalyzer();
		SyntaxAnalyzer(const TTM::LexTable& lextable, const GRB::Greibach& greibach);

		bool Start(Logger& log);
		const std::string dumpTrace() const;

	private:
		enum class RC_STEP
		{
			NS_OK,
			NS_NORULE,
			NS_NORULECHAIN,
			NS_ERROR,
			TS_OK,
			TS_NOK,
			TAPE_END,
			SURPRISE
		};

		struct MfstDiagnosis
		{
			short m_tape_position;
			RC_STEP rc_step;
			short m_nrule;
			short nrule_chain;

			MfstDiagnosis();
			MfstDiagnosis(short m_tape_position, RC_STEP rc_step, short m_nrule, short nrule_chain);
		} diagnosis[MFST_DIAGN_NUMBER];

		GRBALPHABET* m_tape;
		short m_tape_position;
		short m_nrule;
		short m_nrulechain;
		short m_tape_size;
		GRB::Greibach greibach;
		const TTM::LexTable& lextable;
		MFSTSTACK m_stack;
		use_container<std::stack<MfstState>> m_storestate;
		std::stringstream m_trace;

		std::string getCSt();
		std::string getCTape(short pos, short n = 25);
		bool save_state();
		bool restore_state();
		bool push_chain(GRB::Rule::Chain chain);
		RC_STEP step();
		bool savediagnosis(RC_STEP rc_step);
		void printrules();
	};
}