#include <iostream>
#include <sstream>

#include <pqxx/largeobject>
#include <pqxx/transaction>
#include <pqxx/transactor>

#include "test_helpers.hxx"

using namespace pqxx;


// Test program for libpqxx: import file to large object
namespace
{
std::string const Contents{"Large object test contents"};


void test_055()
{
  connection conn;

  largeobject Obj{perform([&conn] {
    char Buf[200];
    work tx{conn};
    largeobjectaccess A{tx, "pqxxlo.txt", std::ios::in};
    auto new_obj{largeobject(A)};
    auto const len{A.read(Buf, sizeof(Buf) - 1)};
    PQXX_CHECK_EQUAL(
      std::string(Buf, std::string::size_type(len)), Contents,
      "Large object contents were mangled.");

    tx.commit();
    return new_obj;
  })};

  perform([&conn, &Obj] {
    work tx{conn};
    Obj.remove(tx);
    tx.commit();
  });
}


PQXX_REGISTER_TEST(test_055);
} // namespace
