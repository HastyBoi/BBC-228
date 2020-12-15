#include "pch.h"
#include "Error.h"

using namespace Error;

//   0 -  99 - системные ошибки
// 100 - 109 - ошибки параметров
// 110 - 119 - ошибки открытия и чтения файлов
// 120 - 140 - ошибки лексического анализатора
// 600 - 610 - ошибки синтаксического анализатора
// 700 - 710 - ошибки семантического анализатора
ERROR errors[ERROR_MAX_ENTRY] = {
	ERROR_ENTRY(0, "Недопустимый код ошибки"),
	ERROR_ENTRY(1, "Системный сбой"),
	ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
	ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
	ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40),
	ERROR_ENTRY_NODEF10(50),
	ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
	ERROR_ENTRY(100, "Параметр -in должен быть задан"),
	ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103),
	ERROR_ENTRY_NODEF(104),
	ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107),
	ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
	ERROR_ENTRY(110, "Ошибка при открытии файла с исходным кодом (-in)"),
	ERROR_ENTRY(111, "Недопустимый символ в исходном файле (-in)"),
	ERROR_ENTRY(112, "Ошибка при создании файла протокола (-log)"),
	ERROR_ENTRY_NODEF(113), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115),
	ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),
	ERROR_ENTRY(120, "Некорректный тип идентификатора"),
	ERROR_ENTRY(121, "Некорректный тип данных"),
	ERROR_ENTRY(122, "Несогласованное число скобок"),
	ERROR_ENTRY(123, "Повторное объявление лексемы"),
	ERROR_ENTRY(124, "Идентификатор не объявлен или объявлен некорректно"),
	ERROR_ENTRY(125, "Рекурсивный вызов функции"),
	ERROR_ENTRY(126, "Попытка инициализации пустой строкой"),
	ERROR_ENTRY_NODEF(127),
	ERROR_ENTRY_NODEF(128),
	ERROR_ENTRY(129, "Неопознанная лексема"),
	ERROR_ENTRY(130, "Отсутствует точка входа"),
	ERROR_ENTRY(131, "Несколько точек входа"),
	ERROR_ENTRY(132, "Несогласованное число кавычек"),
	ERROR_ENTRY_NODEF(133), ERROR_ENTRY_NODEF(134), ERROR_ENTRY_NODEF(135),
	ERROR_ENTRY_NODEF(136), ERROR_ENTRY_NODEF(137), ERROR_ENTRY_NODEF(138),	ERROR_ENTRY_NODEF(139),
	ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
	ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
	ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
	ERROR_ENTRY(600,"Неверная структура программы"),
	ERROR_ENTRY(601,"Ошибочный оператор"),
	ERROR_ENTRY(602,"Ошибка в выражении"),
	ERROR_ENTRY(603,"Ошибка в подвыражении"),
	ERROR_ENTRY(604,"Ошибка в параметрах функции"),
	ERROR_ENTRY(605,"Ошибка в параметрах вызываемой функции"),
	ERROR_ENTRY_NODEF(606), ERROR_ENTRY_NODEF(607), ERROR_ENTRY_NODEF(608),
	ERROR_ENTRY(609, "Неверный номер правила"),
	ERROR_ENTRY_NODEF10(610), ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630),
	ERROR_ENTRY_NODEF10(640), ERROR_ENTRY_NODEF10(650), ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670),
	ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),
	ERROR_ENTRY(700, "Ошибка в возвращаемом значении"),
	ERROR_ENTRY(701, "Ошибка в параметрах функции"),
	ERROR_ENTRY_NODEF(702),
	ERROR_ENTRY_NODEF(703),
	ERROR_ENTRY_NODEF(704),
	ERROR_ENTRY_NODEF(705),
	ERROR_ENTRY(706, "Несоответсвие присваимаего типа данных"),
	ERROR_ENTRY(707, "Неверные типы данных операндов"),
	ERROR_ENTRY_NODEF(708),
	ERROR_ENTRY_NODEF(709),
	ERROR_ENTRY(710, "Ошибка в условной конструкции"),
	ERROR_ENTRY_NODEF(711), ERROR_ENTRY_NODEF(712), ERROR_ENTRY_NODEF(713),ERROR_ENTRY_NODEF(714),ERROR_ENTRY_NODEF(715),
	ERROR_ENTRY_NODEF(716),ERROR_ENTRY_NODEF(717),ERROR_ENTRY_NODEF(718),ERROR_ENTRY_NODEF(719),
	ERROR_ENTRY_NODEF10(720), ERROR_ENTRY_NODEF10(730), ERROR_ENTRY_NODEF10(740),
	ERROR_ENTRY_NODEF10(750), ERROR_ENTRY_NODEF10(760), ERROR_ENTRY_NODEF10(770), ERROR_ENTRY_NODEF10(780),
	ERROR_ENTRY_NODEF10(790), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
};

constexpr ERROR& Error::getErrorByCode(int id) {
	return errors[std::clamp(id, 0, ERROR_MAX_ENTRY)];
}

ERROR Error::getErrorInByCode(int id, int line, int col) {
	ERROR e;

	e = Error::getErrorByCode(id);
	e.inext.line = line;
	e.inext.col = col;

	return e;
}