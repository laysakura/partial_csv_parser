#include <gtest/gtest.h>
#include <PartialCsvParser.hpp>

using namespace PCP;

class PartialCsvParserEdgeCaseTest : public ::testing::Test {
protected:
  PartialCsvParserEdgeCaseTest() {}

  virtual void SetUp() {}
};

TEST_F(PartialCsvParserEdgeCaseTest, wrong_filepath) {
  EXPECT_THROW(CsvConfig("fixture/no_such_file.csv"), PCPError);
}

TEST_F(PartialCsvParserEdgeCaseTest, csv_with_header_has_different_number_of_columns) {
  CsvConfig csv_config("fixture/Invalid_DifferentNumberOfColumns.csv");
  std::vector<std::string> headers = csv_config.get_headers();
  EXPECT_EQ("a", headers[0]); EXPECT_EQ("a", headers[1]); EXPECT_EQ("a", headers[2]);

  PartialCsvParser parser(csv_config);
  std::vector<std::string> row;

  row = parser.get_row();
  EXPECT_EQ("b", row[0]); EXPECT_EQ("b", row[1]); EXPECT_EQ("b", row[1]);
  EXPECT_THROW(parser.get_row(), PCPCsvError);
}

TEST_F(PartialCsvParserEdgeCaseTest, csv_without_header_has_different_number_of_columns) {
  CsvConfig csv_config("fixture/Invalid_DifferentNumberOfColumns.csv", false);
  PartialCsvParser parser(csv_config);
  std::vector<std::string> row;

  row = parser.get_row();
  EXPECT_EQ("a", row[0]); EXPECT_EQ("a", row[1]); EXPECT_EQ("a", row[1]);
  row = parser.get_row();
  EXPECT_EQ("b", row[0]); EXPECT_EQ("b", row[1]); EXPECT_EQ("b", row[1]);
  EXPECT_THROW(parser.get_row(), PCPCsvError);
}

TEST_F(PartialCsvParserEdgeCaseTest, csv_2col_with_continuous_empty_lines) {
  CsvConfig csv_config("fixture/Invalid_2col_ContinuousLastEmptyLines.csv", false);
  PartialCsvParser parser(csv_config);
  std::vector<std::string> row;

  row = parser.get_row();
  EXPECT_EQ("a", row[0]); EXPECT_EQ("a", row[1]);
  row = parser.get_row();
  EXPECT_EQ("b", row[0]); EXPECT_EQ("b", row[1]);
  EXPECT_THROW(parser.get_row(), PCPCsvError);
}

TEST_F(PartialCsvParserEdgeCaseTest, csv_1col_with_continuous_empty_lines) {
  CsvConfig csv_config("fixture/Valid_1col_ContinuousLastEmptyLines.csv", false);
  PartialCsvParser parser(csv_config);
  std::vector<std::string> row;

  row = parser.get_row();
  EXPECT_EQ("a", row[0]);
  row = parser.get_row();
  EXPECT_EQ("b", row[0]);
  EXPECT_NO_THROW(row = parser.get_row());
  EXPECT_EQ("", row[0]);
  EXPECT_TRUE(parser.get_row().empty());
}

TEST_F(PartialCsvParserEdgeCaseTest, empty_csv) {
  // mmap fails with 0 length
  EXPECT_THROW(CsvConfig("fixture/Invalid_Empty.csv", false), PCPError);
}

TEST_F(PartialCsvParserEdgeCaseTest, csv_only_with_newline_without_header) {
  CsvConfig csv_config("fixture/Valid_OnlyNewline.csv", false);
  PartialCsvParser parser(csv_config);
  EXPECT_EQ("", parser.get_row()[0]);
  EXPECT_TRUE(parser.get_row().empty());
}

TEST_F(PartialCsvParserEdgeCaseTest, csv_only_with_newline_with_header) {
  CsvConfig csv_config("fixture/Valid_OnlyNewline.csv");
  std::vector<std::string> headers = csv_config.get_headers();
  EXPECT_EQ("", headers[0]);

  PartialCsvParser parser(csv_config);
  EXPECT_TRUE(parser.get_row().empty());
}

TEST_F(PartialCsvParserEdgeCaseTest, csv_with_empty_columns) {
  CsvConfig csv_config("fixture/Valid_WithEmptyColumns.csv", false);
  PartialCsvParser parser(csv_config);
  std::vector<std::string> row;

  row = parser.get_row();
  EXPECT_EQ("a", row[0]); EXPECT_EQ("a", row[1]);
  row = parser.get_row();
  EXPECT_EQ("", row[0]); EXPECT_EQ("", row[1]);
  row = parser.get_row();
  EXPECT_EQ("", row[0]); EXPECT_EQ("c", row[1]);
  row = parser.get_row();
  EXPECT_EQ("d", row[0]); EXPECT_EQ("", row[1]);
  EXPECT_TRUE(parser.get_row().empty());
}
