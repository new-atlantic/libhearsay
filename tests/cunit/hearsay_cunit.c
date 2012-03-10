/*
 * Copyright 2012 Torsti Schulz
 *
 * This file is part of the Hearsay library.
 *
 * Hearsay is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hearsay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

// hearsay_message suite


int init_hearsay_message_suite (void)
{
	return 0;
}

int clean_hearsay_message_suite (void)
{
	return 0;
}

void test_hearsay_message_validation (void)
{
	CU_ASSERT (0 == 1);
}

void test_hearsay_message_hash_calculation (void)
{
	CU_ASSERT (0 == 1);
}

void test_hearsay_message_hash_validation (void)
{
	CU_ASSERT (0 == 1);
}


// text/hearsay suite

int init_hearsay_text_suite (void)
{
	return 0;
}

int clean_hearsay_text_suite (void)
{
	return 0;
}

void test_hearsay_text_validation (void)
{
	CU_ASSERT (0 == 1);
}

void test_hearsay_text_to_struct (void)
{
	CU_ASSERT (0 == 1);
}

void test_hearsay_struct_to_text (void)
{
	CU_ASSERT (0 == 1);
}

int main (void)
{

	CU_pSuite hearsay_text_suite = NULL;
	CU_pSuite hearsay_message_suite = NULL;

	if (CU_initialize_registry() != CUE_SUCCESS)
		return CU_get_error();

	hearsay_message_suite = CU_add_suite("hearsay_message test suite",
	                                  init_hearsay_message_suite,
	                                  clean_hearsay_message_suite);
	if (NULL == hearsay_message_suite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_add_test(hearsay_message_suite,
	            "hearsay_message validation",
	            test_hearsay_message_validation);

	CU_add_test(hearsay_message_suite,
	            "hearsay_message hash calculation",
	            test_hearsay_message_hash_calculation);

	CU_add_test(hearsay_message_suite,
	            "hearsay_message hash validation",
	            test_hearsay_message_hash_validation);

	hearsay_text_suite = CU_add_suite("text/hearsay function test suite",
	                                  init_hearsay_text_suite,
	                                  clean_hearsay_text_suite);
	if (NULL == hearsay_text_suite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_add_test(hearsay_text_suite,
	            "test 'text/hearsay' validation",
	            test_hearsay_text_validation);

	CU_add_test(hearsay_text_suite,
	            "test 'text/hearsay' to struct transformation",
	            test_hearsay_text_to_struct);

	CU_add_test(hearsay_text_suite,
	            "test struct to 'text/hearsay' transformation ",
	            test_hearsay_struct_to_text);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	
	CU_cleanup_registry();
}
