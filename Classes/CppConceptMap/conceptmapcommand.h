#ifndef CONCEPTMAPCOMMAND_H
#define CONCEPTMAPCOMMAND_H

#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>

#include "conceptmapfwd.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Command can be used to do and undo commands to a concept map Widget
struct Command
{
  virtual ~Command() noexcept {}
  bool CanDoCommand(const Widget * const widget) const noexcept;
  void DoCommand(Widget * const widget) noexcept;
  virtual const std::string ToStr() const noexcept = 0;
  virtual void Undo() noexcept = 0;

  private:
  ///Hook
  virtual bool CanDoCommandSpecific(const Widget * const widget) const noexcept = 0;

  ///Hook
  virtual void DoCommandSpecific(Widget * const widget) noexcept = 0;
};

///Delete a concept map
///-Can be used only when there is an existing concept map
struct CommandDeleteConceptMap : public Command
{
  CommandDeleteConceptMap() : m_deleted_concept_map{}, m_widget{} {}
  CommandDeleteConceptMap(const CommandDeleteConceptMap&) = delete;
  CommandDeleteConceptMap& operator=(const CommandDeleteConceptMap&) = delete;
  ~CommandDeleteConceptMap() noexcept {}
  bool CanDoCommandSpecific(const Widget * const widget) const noexcept;
  void DoCommandSpecific(Widget * const widget) noexcept;
  const std::string ToStr() const noexcept { return "delete concept map"; }
  void Undo() noexcept;

  private:
  boost::shared_ptr<ConceptMap> m_deleted_concept_map;
  Widget * m_widget;

};

///Start a new concept map
///-Can be used only when there is no existing concept map
struct CommandStartConceptMap : public Command
{
  CommandStartConceptMap() : m_widget{} {}
  CommandStartConceptMap(const CommandStartConceptMap&) = delete;
  CommandStartConceptMap& operator=(const CommandStartConceptMap&) = delete;
  ~CommandStartConceptMap() noexcept {}

  bool CanDoCommandSpecific(const Widget * const widget) const noexcept;
  void DoCommandSpecific(Widget * const widget) noexcept;
  const std::string ToStr() const noexcept { return "start concept map"; }
  void Undo() noexcept;

  private:
  Widget * m_widget;
};

} //~namespace cmap
} //~namespace ribi


#endif // CONCEPTMAPCOMMAND_H
