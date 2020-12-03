#pragma once
#include <stack>
#include "Greibach.h"
#include "LexTable.h"
#include "Error.h"
#include "Logger.h"

#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

#define TRACE

#ifdef TRACE

#define MFST_TRACE_START(log) log << std::setw(4)<<std::left<<"Шаг"<<": " \
	<< std::setw(30) << std::left << "Правило"  \
	<< std::setw(30) << std::left << "Входная лента" \
	<< std::setw(20) << std::left << "Стек" \
	<< '\n';

#define MFST_TRACE1(log) log <<std::setw(4)<<std::left<<++FST_TRACE_n<<": " \
	<< std::setw(30) << std::left << rule.getCRule(nrulechain)  \
	<< std::setw(30) << std::left << getCTape(tape_position) \
	<< std::setw(20) << std::left << getCSt() \
	<< '\n';

#define MFST_TRACE2(log)    log <<std::setw(4)<<std::left<<FST_TRACE_n<<": " \
	<< std::setw(30) << std::left << " "  \
	<< std::setw(30) << std::left << getCTape(tape_position) \
	<< std::setw(20) << std::left << getCSt() \
	<< '\n';

#define MFST_TRACE3(log)     log<<std::setw(4)<<std::left<<++FST_TRACE_n<<": " \
	<< std::setw(30) << std::left << " "  \
	<< std::setw(30) << std::left << getCTape(tape_position) \
	<< std::setw(20) << std::left << getCSt() \
	<< '\n';

#define MFST_TRACE4(log,c) log<<std::setw(4)<<std::left<<++FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<'\n';
#define MFST_TRACE5(log,c) log<<std::setw(4)<<std::left<<  FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<'\n';

#define MFST_TRACE6(log,c,k) log<<std::setw(4)<<std::left<<++FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<k<<'\n';

#define MFST_TRACE7(log)  log<<std::setw(4)<<std::left<<state.tape_position<<": "\
	<< std::setw(20) << std::left << rule.getCRule(state.nrulechain) \
	<< '\n';

#else

#define MFST_TRACE_START(log) ;
#define MFST_TRACE1(log) ;
#define MFST_TRACE2(log) ;
#define MFST_TRACE3(log) ;
#define MFST_TRACE4(log,c) ;
#define MFST_TRACE5(log,c) ;
#define MFST_TRACE6(log,c,k) ;
#define MFST_TRACE7(log) ;

#endif // TRACE

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
		short tape_position;
		short nrule;
		short nrulechain;
		MFSTSTACK st;

		MfstState();
		MfstState(short position, MFSTSTACK st, short nrulechain);
		MfstState(short position, MFSTSTACK st, short nrule, short nrulechain);
	};

	struct Mfst
	{
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
			short tape_position;
			RC_STEP rc_step;
			short nrule;
			short nrule_chain;

			MfstDiagnosis();
			MfstDiagnosis(short tape_position, RC_STEP rc_step, short nrule, short nrule_chain);
		} diagnosis[MFST_DIAGN_NUMBER];

		GRBALPHABET* tape;
		short tape_position;
		short nrule;
		short nrulechain;
		short tape_size;
		GRB::Greibach greibach;
		const TTM::LexTable& lextable;
		MFSTSTACK st;
		use_container<std::stack<MfstState>> storestate;

		Mfst();
		Mfst(const TTM::LexTable& lextable, const GRB::Greibach& greibach);

		std::string getCSt();
		std::string getCTape(short pos, short n = 25);
		std::string getDiagnosis(short n);
		bool save_state(TTM::Logger& log);
		bool restore_state(TTM::Logger& log);
		bool push_chain(GRB::Rule::Chain chain);
		RC_STEP step(TTM::Logger& log);
		bool start(TTM::Logger& log);
		bool savediagnosis(RC_STEP rc_step);
		void printrules(TTM::Logger& log);
	};
}