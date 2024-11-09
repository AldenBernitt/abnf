#include <stdio.h>
#include "tests.c"

#define FOUT stdout

typedef struct {
	const char* name;
	_Bool(*test)();
} testcase;

int main()
{
	testcase cases[144];

	cases[0].name = "define_name_alpha";
	cases[0].test = define_name_alpha;

	cases[1].name = "define_name_lots_of_hyphens";
	cases[1].test = define_name_lots_of_hyphens;

	cases[2].name = "define_name_lots_of_digits";
	cases[2].test = define_name_lots_of_digits;

	cases[3].name = "define_name_digits_and_hyphens";
	cases[3].test = define_name_digits_and_hyphens;

	cases[4].name = "define_name_parse_error_leading_space";
	cases[4].test = define_name_parse_error_leading_space;

	cases[5].name = "define_definedas_inc";
	cases[5].test = define_definedas_inc;

	cases[6].name = "define_definedas_with_spaces";
	cases[6].test = define_definedas_with_spaces;

	cases[7].name = "define_definedas_with_tabs";
	cases[7].test = define_definedas_with_tabs;

	cases[8].name = "define_definedas_with_crlf";
	cases[8].test = define_definedas_with_crlf;

	cases[9].name = "define_definedas_with_space_tab_crlf";
	cases[9].test = define_definedas_with_space_tab_crlf;

	cases[10].name = "define_definedas_with_comments";
	cases[10].test = define_definedas_with_comments;

	cases[11].name = "define_definedas_with_comments_space_tab_crlf";
	cases[11].test = define_definedas_with_comments_space_tab_crlf;

	cases[12].name = "define_definedas_parse_error_crlf_no_wsp";
	cases[12].test = define_definedas_parse_error_crlf_no_wsp;

	cases[13].name = "define_definedas_parse_error_comment_no_wsp";
	cases[13].test = define_definedas_parse_error_comment_no_wsp;

	cases[14].name = "define_definedas_parse_error_many_comments_no_wsp";
	cases[14].test = define_definedas_parse_error_many_comments_no_wsp;

	cases[15].name = "define_elements_charval";
	cases[15].test = define_elements_charval;

	cases[16].name = "define_elements_charval_empty";
	cases[16].test = define_elements_charval_empty;

	cases[17].name = "define_elements_charval_multiple";
	cases[17].test = define_elements_charval_multiple;

	cases[18].name = "define_elements_charval_parse_error_no_open_quote";
	cases[18].test = define_elements_charval_parse_error_no_open_quote;

	cases[19].name = "define_elements_charval_parse_error_no_close_quote";
	cases[19].test = define_elements_charval_parse_error_no_close_quote;

	cases[20].name = "define_elements_charval_parse_error_invalid_char";
	cases[20].test = define_elements_charval_parse_error_invalid_char;

	cases[21].name = "define_elements_binval";
	cases[21].test = define_elements_binval;

	cases[22].name = "define_elements_binval_multiple";
	cases[22].test = define_elements_binval_multiple;

	cases[23].name = "define_elements_binval_concat";
	cases[23].test = define_elements_binval_concat;

	cases[24].name = "define_elements_binval_range";
	cases[24].test = define_elements_binval_range;

	cases[25].name = "define_elements_binval_invalid";
	cases[25].test = define_elements_binval_invalid;

	cases[26].name = "define_elements_binval_parse_error_start_concat";
	cases[26].test = define_elements_binval_parse_error_start_concat;

	cases[27].name = "define_elements_binval_parse_error_end_concat";
	cases[27].test = define_elements_binval_parse_error_end_concat;

	cases[28].name = "define_elements_binval_parse_error_invalid_concat";
	cases[28].test = define_elements_binval_parse_error_invalid_concat;

	cases[29].name = "define_elements_binval_parse_error_invalid_double_concat";
	cases[29].test = define_elements_binval_parse_error_invalid_double_concat;

	cases[30].name = "define_elements_binval_parse_error_start_range";
	cases[30].test = define_elements_binval_parse_error_start_range;

	cases[31].name = "define_elements_binval_parse_error_end_range";
	cases[31].test = define_elements_binval_parse_error_end_range;

	cases[32].name = "define_elements_binval_parse_error_invalid_range";
	cases[32].test = define_elements_binval_parse_error_invalid_range;

	cases[33].name = "define_elements_binval_parse_error_double_range";
	cases[33].test = define_elements_binval_parse_error_double_range;

	cases[34].name = "define_elements_decval";
	cases[34].test = define_elements_decval;

	cases[35].name = "define_elements_decval_multiple";
	cases[35].test = define_elements_decval_multiple;

	cases[36].name = "define_elements_decval_concat";
	cases[36].test = define_elements_decval_concat;

	cases[37].name = "define_elements_decval_range";
	cases[37].test = define_elements_decval_range;

	cases[38].name = "define_elements_decval_invalid";
	cases[38].test = define_elements_decval_invalid;

	cases[39].name = "define_elements_decval_parse_error_start_concat";
	cases[39].test = define_elements_decval_parse_error_start_concat;

	cases[40].name = "define_elements_decval_parse_error_end_concat";
	cases[40].test = define_elements_decval_parse_error_end_concat;

	cases[41].name = "define_elements_decval_parse_error_invalid_concat";
	cases[41].test = define_elements_decval_parse_error_invalid_concat;

	cases[42].name = "define_elements_decval_parse_error_double_concat";
	cases[42].test = define_elements_decval_parse_error_double_concat;

	cases[43].name = "define_elements_decval_parse_error_start_range";
	cases[43].test = define_elements_decval_parse_error_start_range;

	cases[44].name = "define_elements_decval_parse_error_end_range";
	cases[44].test = define_elements_decval_parse_error_end_range;

	cases[45].name = "define_elements_decval_parse_error_invalid_range";
	cases[45].test = define_elements_decval_parse_error_invalid_range;

	cases[46].name = "define_elements_decval_parse_error_double_range";
	cases[46].test = define_elements_decval_parse_error_double_range;

	cases[47].name = "define_elements_hexval";
	cases[47].test = define_elements_hexval;

	cases[48].name = "define_elements_hexval_multiple";
	cases[48].test = define_elements_hexval_multiple;

	cases[49].name = "define_elements_hexval_concat";
	cases[49].test = define_elements_hexval_concat;

	cases[50].name = "define_elements_hexval_range";
	cases[50].test = define_elements_hexval_range;

	cases[51].name = "define_elements_hexval_invalid";
	cases[51].test = define_elements_hexval_invalid;

	cases[52].name = "define_elements_hexval_parse_error_start_concat";
	cases[52].test = define_elements_hexval_parse_error_start_concat;

	cases[53].name = "define_elements_hexval_parse_error_end_concat";
	cases[53].test = define_elements_hexval_parse_error_end_concat;

	cases[54].name = "define_elements_hexval_parse_error_invalid_concat";
	cases[54].test = define_elements_hexval_parse_error_invalid_concat;

	cases[55].name = "define_elements_hexval_parse_error_double_concat";
	cases[55].test = define_elements_hexval_parse_error_double_concat;

	cases[56].name = "define_elements_hexval_parse_error_start_range";
	cases[56].test = define_elements_hexval_parse_error_start_range;

	cases[57].name = "define_elements_hexval_parse_error_end_range";
	cases[57].test = define_elements_hexval_parse_error_end_range;

	cases[58].name = "define_elements_hexval_parse_error_invalid_range";
	cases[58].test = define_elements_hexval_parse_error_invalid_range;

	cases[59].name = "define_elements_hexval_parse_error_double_range";
	cases[59].test = define_elements_hexval_parse_error_double_range;

	cases[60].name = "define_elements_proseval";
	cases[60].test = define_elements_proseval;

	cases[61].name = "define_elements_proseval_empty";
	cases[61].test = define_elements_proseval_empty;

	cases[62].name = "define_elements_proseval_multiple";
	cases[62].test = define_elements_proseval_multiple;

	cases[63].name = "define_elements_proseval_parse_error_invalid";
	cases[63].test = define_elements_proseval_parse_error_invalid;

	cases[64].name = "define_elements_proseval_parse_error_close_bracket";
	cases[64].test = define_elements_proseval_parse_error_close_bracket;

	cases[65].name = "define_elements_name";
	cases[65].test = define_elements_name;

	cases[66].name = "define_elements_name_alpha";
	cases[66].test = define_elements_name_alpha;

	cases[67].name = "define_elements_name_digits";
	cases[67].test = define_elements_name_digits;

	cases[68].name = "define_elements_name_hypens";
	cases[68].test = define_elements_name_hypens;

	cases[69].name = "define_elements_name_digits_and_hyphens";
	cases[69].test = define_elements_name_digits_and_hyphens;

	cases[70].name = "define_elements_name_parse_error_start_hyphen";
	cases[70].test = define_elements_name_parse_error_start_hyphen;

	cases[71].name = "define_elements_name_parse_error_empty";
	cases[71].test = define_elements_name_parse_error_empty;

	cases[72].name = "define_elements_repetition";
	cases[72].test = define_elements_repetition;

	cases[73].name = "define_elements_repetition_multiple_digits";
	cases[73].test = define_elements_repetition_multiple_digits;

	cases[74].name = "define_elements_repetition_no_upper";
	cases[74].test = define_elements_repetition_no_upper;

	cases[75].name = "define_elements_repetition_no_upper_multiple_digits";
	cases[75].test = define_elements_repetition_no_upper_multiple_digits;

	cases[76].name = "define_elements_repetition_no_lower";
	cases[76].test = define_elements_repetition_no_lower;

	cases[77].name = "define_elements_repetition_no_lower_multiple_digits";
	cases[77].test = define_elements_repetition_no_lower_multiple_digits;

	cases[78].name = "define_elements_repetition_lower_and_upper";
	cases[78].test = define_elements_repetition_lower_and_upper;

	cases[79].name = "define_elements_repetition_lower_and_upper_multiple_digits";
	cases[79].test = define_elements_repetition_lower_and_upper_multiple_digits;

	cases[80].name = "define_elements_repetition_invalid";
	cases[80].test = define_elements_repetition_invalid;

	cases[81].name = "define_elements_repetition_parse_error_double_star";
	cases[81].test = define_elements_repetition_parse_error_double_star;

	cases[82].name = "define_elements_repetition_parse_error_multiple_stars";
	cases[82].test = define_elements_repetition_parse_error_multiple_stars;

	cases[83].name = "define_elements_repetition_parse_error_multiple_stars2";
	cases[83].test = define_elements_repetition_parse_error_multiple_stars2;

	cases[84].name = "define_elements_repetition_parse_error_invalid_number";
	cases[84].test = define_elements_repetition_parse_error_invalid_number;

	cases[85].name = "define_elements_repetition_parse_error_contains_spaces";
	cases[85].test = define_elements_repetition_parse_error_contains_spaces;

	cases[86].name = "define_elements_repetition_parse_error_contains_spaces2";
	cases[86].test = define_elements_repetition_parse_error_contains_spaces2;

	cases[87].name = "define_elements_repetition_parse_error_contains_spaces3";
	cases[87].test = define_elements_repetition_parse_error_contains_spaces3;

	cases[88].name = "define_elements_concatenation";
	cases[88].test = define_elements_concatenation;

	cases[89].name = "define_elements_concatenation_multiple";
	cases[89].test = define_elements_concatenation_multiple;

	cases[90].name = "define_elements_concatenation_multiple_spaces";
	cases[90].test = define_elements_concatenation_multiple_spaces;

	cases[91].name = "define_elements_alternation";
	cases[91].test = define_elements_alternation;

	cases[92].name = "define_elements_alternation_multiple";
	cases[92].test = define_elements_alternation_multiple;

	cases[93].name = "define_elements_alternation_no_spaces";
	cases[93].test = define_elements_alternation_no_spaces;

	cases[94].name = "define_elements_alternation_mixed_spaces";
	cases[94].test = define_elements_alternation_mixed_spaces;

	cases[95].name = "define_elements_alternation_multiple_spaces";
	cases[95].test = define_elements_alternation_multiple_spaces;

	cases[96].name = "define_elements_altnernation_newlines";
	cases[96].test = define_elements_altnernation_newlines;

	cases[97].name = "define_elements_group";
	cases[97].test = define_elements_group;

	cases[98].name = "define_elements_group_nested";
	cases[98].test = define_elements_group_nested;

	cases[99].name = "define_elements_group_nested_mixed_spaces";
	cases[99].test = define_elements_group_nested_mixed_spaces;

	cases[100].name = "define_elements_group_no_spaces";
	cases[100].test = define_elements_group_no_spaces;

	cases[101].name = "define_elements_group_parse_error_empty";
	cases[101].test = define_elements_group_parse_error_empty;

	cases[102].name = "define_elements_group_parse_error_empty_nested";
	cases[102].test = define_elements_group_parse_error_empty_nested;

	cases[103].name = "define_elements_group_parse_error_no_close";
	cases[103].test = define_elements_group_parse_error_no_close;

	cases[104].name = "define_elements_group_parse_error_mismatched_brackets";
	cases[104].test = define_elements_group_parse_error_mismatched_brackets;

	cases[105].name = "define_elements_group_parse_error_different_brackets";
	cases[105].test = define_elements_group_parse_error_different_brackets;

	cases[106].name = "define_elements_option";
	cases[106].test = define_elements_option;

	cases[107].name = "define_elements_option_nested";
	cases[107].test = define_elements_option_nested;

	cases[108].name = "define_elements_option_nested_mixed_spaces";
	cases[108].test = define_elements_option_nested_mixed_spaces;

	cases[109].name = "define_elements_option_no_spaces";
	cases[109].test = define_elements_option_no_spaces;

	cases[110].name = "define_elements_option_parse_error_empty";
	cases[110].test = define_elements_option_parse_error_empty;

	cases[111].name = "define_elements_option_parse_error_empty_nested";
	cases[111].test = define_elements_option_parse_error_empty_nested;

	cases[112].name = "define_elements_option_parse_error_no_close";
	cases[112].test = define_elements_option_parse_error_no_close;

	cases[113].name = "define_elements_option_parse_error_mismatched_brackets";
	cases[113].test = define_elements_option_parse_error_mismatched_brackets;

	cases[114].name = "define_elements_option_parse_error_different_brackets";
	cases[114].test = define_elements_option_parse_error_different_brackets;

	cases[115].name = "define_generic";
	cases[115].test = define_generic;

	cases[116].name = "define_generic1";
	cases[116].test = define_generic1;

	cases[117].name = "define_generic2";
	cases[117].test = define_generic2;

	cases[118].name = "define_generic3";
	cases[118].test = define_generic3;

	cases[119].name = "define_generic4";
	cases[119].test = define_generic4;

	cases[120].name = "define_generic5";
	cases[120].test = define_generic5;

	cases[121].name = "define_generic6";
	cases[121].test = define_generic6;

	cases[122].name = "define_generic7";
	cases[122].test = define_generic7;

	cases[123].name = "define_generic8";
	cases[123].test = define_generic8;

	cases[124].name = "define_generic9";
	cases[124].test = define_generic9;

	cases[125].name = "define_generic10";
	cases[125].test = define_generic10;

	cases[126].name = "define_generic11";
	cases[126].test = define_generic11;

	cases[127].name = "define_generic12";
	cases[127].test = define_generic12;

	cases[128].name = "define_generic13";
	cases[128].test = define_generic13;

	cases[129].name = "define_generic14";
	cases[129].test = define_generic14;

	cases[130].name = "define_generic15";
	cases[130].test = define_generic15;

	cases[131].name = "define_generic16";
	cases[131].test = define_generic16;

	cases[132].name = "define_generic17";
	cases[132].test = define_generic17;

	cases[133].name = "define_generic18";
	cases[133].test = define_generic18;

	cases[134].name = "define_generic19";
	cases[134].test = define_generic19;

	cases[135].name = "define_multiple_lines";
	cases[135].test = define_multiple_lines;

	cases[136].name = "define_trailing_wsp";
	cases[136].test = define_trailing_wsp;

	cases[137].name = "define_multiple_defines";
	cases[137].test = define_multiple_defines;

	cases[138].name = "define_parse_error_empty";
	cases[138].test = define_parse_error_empty;

	cases[139].name = "define_parse_error_trailing_character";
	cases[139].test = define_parse_error_trailing_character;

	cases[140].name = "define_parse_error_nl_nospace";
	cases[140].test = define_parse_error_nl_nospace;

	cases[141].name = "define_option_nolim";
	cases[141].test = define_option_nolim;

	cases[142].name = "define_value_upperlimit";
	cases[142].test = define_value_upperlimit;

	cases[143].name = "define_error_double_define";
	cases[143].test = define_error_double_define;

	size_t passes = 0;
	size_t total = 144;
	for (size_t i = 0; i < 144; i++) {
		fprintf(FOUT, "%s...", cases[i].name);
		if (cases[i].test()) {
			fprintf(FOUT, "[32m" "PASSED" "[0m" "\n");
			passes++;
		}
		else {
			fprintf(FOUT, "[31m" "FAILED" "[0m" "\n");
		}
	}
	fprintf(FOUT, "\nTesting complete (%lu/%lu passed).\n", passes, total);

	return 0;
}
