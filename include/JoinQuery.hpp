#include <string>
#include<unordered_map>
#include<unordered_set>

//---------------------------------------------------------------------------
class JoinQuery
/// Class which provides answers to queries on lineitem orders and customer
{
   public:
  /**************************************************************************
   *  The constructor receives paths to data from the TPC-H benchmark.
   *  You can explore an example dataset from this benchmark on
   *  http://www.hyper-db.de/size_terface.html
   *  Furthermore, another example dataset is provided in
   *  test/data/tpch/sf0_001
   *
   *  Parameters to constructor:
   *     lineitem: path to a lineitem.tbl
   *     orders: path to a orders.tbl
   *     customer: path to a customer.tbl
   ************************************************************************/
   JoinQuery(std::string lineitem, std::string orders, std::string customer);

  /**************************************************************************
    *  Computes avg(l_quantity) for the following query.
    *
    *  select avg(l_quantity)
    *  from lineitem, orders, customer
    *  where
    *   l_orderkey = o_orderkey and
    *   o_custkey = c_custkey and
    *   c_mktsegment = <segmentParam>
    *
    *  where the tables lineitem, orders and customer are those identified by
    *the paths given in the constructor.
    *
    *  Parameters:
    *     segmentParam: string to use instead of <segmentParam> in query
    *
    *  Returns: avg(l_quantity) * 100
    *     In the data files, l_quantity is of fixed posize_t type with
    *     two digits right of the posize_t. Thus avg(l_quantity) shouls
    *     also have two digits right of the posize_t. Therefore
    *     avg(l_quantity) * 100 is the same with the posize_t dropped.
    ************************************************************************/
   size_t avg(std::string segmentParam);
   /// Returns line count of given file
   size_t lineCount(std::string rel);

   std::unordered_map<std::string, std::unordered_set<size_t>> custmap_gen(std::string cPath);
   std::unordered_map<std::string,size_t> avgmap_gen(std::string lPath, std::unordered_map<std::string, std::unordered_set<size_t>> ordermap);
   std::unordered_set<size_t> ordermap_gen(std::string oPath,std::unordered_set<size_t> cust_segment_set);
};
//---------------------------------------------------------------------------
