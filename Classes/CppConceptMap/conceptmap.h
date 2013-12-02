#ifndef CONCEPTMAPCONCEPTMAP_H
#define CONCEPTMAPCONCEPTMAP_H

#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include "conceptmapfwd.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

struct ConceptMapFactory;

struct ConceptMap
{
  ConceptMap(const ConceptMap&) = delete;
  ConceptMap& operator=(const ConceptMap&) = delete;

  //Add an Edge, assumes that the nodes it points to are in the concept map
  void AddEdge(const boost::shared_ptr<ribi::cmap::Edge> edge);

  //Add a node, always works
  void AddNode(const boost::shared_ptr<ribi::cmap::Node> node);

  ///Test if this ConceptMap can be constructed successfully
  static bool CanConstruct(
    const std::vector<boost::shared_ptr<ribi::cmap::Node> >& nodes,
    const std::vector<boost::shared_ptr<ribi::cmap::Edge> >& edges);

  ///Prepend the question as a first node, before adding the supplied nodes
  static const std::vector<boost::shared_ptr<ribi::cmap::Node> > CreateNodes(
    const std::string& question,
    const std::vector<boost::shared_ptr<ribi::cmap::Node> >& nodes);

  ///Create all sub-conceptmaps
  ///Note that CreateSubs()[0] is the concept map around the focal question
  const std::vector<boost::shared_ptr<ribi::cmap::ConceptMap> > CreateSubs() const;

  ///Delete an edge
  void DeleteEdge(const boost::shared_ptr<ribi::cmap::Edge> edge);

  ///Delete a node and all the edges connected to it
  void DeleteNode(const boost::shared_ptr<ribi::cmap::Node> node);

  ///Check if the ConceptMap is empty
  bool Empty() const;

  ///Get the edges
  const std::vector<boost::shared_ptr<const cmap::Edge> >  GetEdges() const;
  const std::vector<boost::shared_ptr<      Edge> >& GetEdges() { return m_edges; }

  ///Get the nodes
  const std::vector<boost::shared_ptr<const cmap::Node> >  GetNodes() const;
  const std::vector<boost::shared_ptr<      Node> >& GetNodes() { return m_nodes; }

  ///Get the focus question
  const std::string GetQuestion() const;

  ///Similar to operator==, except that the GUI member variables aren't checked for equality
  static bool HasSameContent(const ribi::cmap::ConceptMap& lhs, const ribi::cmap::ConceptMap& rhs);

  #ifndef NDEBUG
  ///Check if there are no nulls in the edges and nodes
  bool IsValid() const;
  #endif

  ///Convert a ConceptMap from an XML std::string
  static const std::string ToXml(const boost::shared_ptr<const ribi::cmap::ConceptMap> c);

private:

  ///The edges
  std::vector<boost::shared_ptr<ribi::cmap::Edge> > m_edges;

  ///The nodes
  std::vector<boost::shared_ptr<ribi::cmap::Node> > m_nodes;

  #ifndef NDEBUG
  ///Test this class
  static void Test();
  #endif

  ///Block constructor, except for the friend ConceptMapFactory
  ConceptMap(const std::string& question);
  //Nodes[0] must be the focal question
  ConceptMap(
    const std::vector<boost::shared_ptr<ribi::cmap::Node> >& nodes,
    const std::vector<boost::shared_ptr<ribi::cmap::Edge> >& edges = {});
  ///Create a concept map from a cluster
  #ifdef TO_ADD_TO_PROJECTBRAINWEAVER
  ConceptMap(
    const std::string& question,
    const boost::shared_ptr<pvdb::Cluster>& cluster);
  #endif
  friend ConceptMapFactory;
  ///Block destructor, except for the friend boost::checked_delete
  ~ConceptMap() {}
  friend void boost::checked_delete<>(ConceptMap* x);
};

bool IsEqual(const ribi::cmap::ConceptMap& lhs, const ribi::cmap::ConceptMap& rhs);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCONCEPTMAP_H