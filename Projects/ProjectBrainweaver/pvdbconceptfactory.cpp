#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "pvdbconceptfactory.h"

#include "pvdbcompetency.h"
#include "pvdbconcept.h"
#include "pvdbexample.h"
#include "pvdbexamples.h"
#include "pvdbexamplefactory.h"
#include "pvdbexamplesfactory.h"
#pragma GCC diagnostic push

const boost::shared_ptr<ribi::pvdb::Concept> ribi::pvdb::ConceptFactory::Create(
  const std::string& name,
  const boost::shared_ptr<ribi::pvdb::Examples>& examples,
  const bool is_complex,
  const int rating_complexity,
  const int rating_concreteness,
  const int rating_specificity)
{
  assert(examples);
  assert(rating_complexity >= -1);
  assert(rating_complexity <=  2);

  boost::shared_ptr<ribi::pvdb::Concept> concept(
    new Concept(
      name,
      examples,
      is_complex,
      rating_complexity,
      rating_concreteness,
      rating_specificity
    )
  );
  assert(concept);
  return concept;
}

#ifndef NDEBUG
const boost::shared_ptr<ribi::pvdb::Concept> ribi::pvdb::ConceptFactory::DeepCopy(
  const boost::shared_ptr<const ribi::pvdb::Concept>& concept)
{
  const boost::shared_ptr<ribi::pvdb::Examples> examples
    = ExamplesFactory::Create(concept->GetExamples());
  assert(examples);
  assert(IsEqual(*examples,*concept->GetExamples()));

  assert(concept->GetRatingComplexity() >= -1);
  assert(concept->GetRatingComplexity() <=  2);

  const boost::shared_ptr<ribi::pvdb::Concept> q
    = Create(
      concept->GetName(),
      examples,
      concept->GetIsComplex(),
      concept->GetRatingComplexity(),
      concept->GetRatingConcreteness(),
      concept->GetRatingSpecificity());
  assert(q);
  assert(q->GetExamples());
  assert(IsEqual(*concept,*q));
  return q;
}
#endif

//Do not create this version: it will lead to ambiguities
//static const boost::shared_ptr<ribi::pvdb::Concept> ribi::pvdb::ConceptFactory::Create(
//  const std::string& name,
//  const std::vector<boost::shared_ptr<const pvdb::Example> >& examples)
//{
//  return Create(name,ExamplesFactory::Create(examples));
//}

const boost::shared_ptr<ribi::pvdb::Concept> ribi::pvdb::ConceptFactory::Create(
  const std::string& name,
  const std::vector<std::pair<std::string,Competency> >& v,
  const bool is_complex,
  const int rating_complexity,
  const int rating_concreteness,
  const int rating_specificity)
{
  assert(rating_complexity >= -1);
  assert(rating_complexity <=  2);

  std::vector<boost::shared_ptr<pvdb::Example> > w;
  std::transform(v.begin(),v.end(),std::back_inserter(w),
    [](const std::pair<std::string,Competency>& p)
    {
      const boost::shared_ptr<pvdb::Example> q
        = ExampleFactory::Create(
          p.first,
          p.second);
      assert(q);
      return q;
    }
  );

  const boost::shared_ptr<ribi::pvdb::Examples> examples
    = ExamplesFactory::Create(w);
  assert(examples);

  const boost::shared_ptr<ribi::pvdb::Concept> concept
    = Create(
    name,
    examples,
    is_complex,
    rating_complexity,
    rating_concreteness,
    rating_specificity);
  assert(concept);
  return concept;
}

const std::vector<boost::shared_ptr<ribi::pvdb::Concept> > ribi::pvdb::ConceptFactory::GetTests()
{
  std::vector<boost::shared_ptr<ribi::pvdb::Concept> > v(5);
  {
    const boost::shared_ptr<ribi::pvdb::Examples> examples = ExamplesFactory::Create();
    assert(examples);
    const boost::shared_ptr<ribi::pvdb::Concept> p = Create("Concept without examples", examples, false, 0, 1, 2);
    assert(p);
    assert(p->GetRatingComplexity() >= -1);
    assert(p->GetRatingComplexity() <=  2);
    v[0] = p;
  }
  {
    const boost::shared_ptr<ribi::pvdb::Concept> p = Create("Concept with one example", { { "Only example", pvdb::Competency::profession } }, 1, 2, 0);
    assert(p);
    assert(p->GetRatingComplexity() >= -1);
    assert(p->GetRatingComplexity() <=  2);
    v[1] = p;
  }
  {
    const boost::shared_ptr<ribi::pvdb::Concept> p = Create("Concept with two examples", { { "First example", pvdb::Competency::organisations }, { "Second example", pvdb::Competency::social_surroundings } }, 2, 0, 1);
    assert(p);
    assert(p->GetRatingComplexity() >= -1);
    assert(p->GetRatingComplexity() <=  2);
    v[2] = p;
  }
  {
    const boost::shared_ptr<ribi::pvdb::Concept> p = Create("Concept with three examples", { { "Example 1 of 3", pvdb::Competency::target_audience }, { "Example 2 of 3", pvdb::Competency::ti_knowledge }, { "Example 3 of 3", pvdb::Competency::prof_growth } }, 0, 1, 2);
    assert(p);
    assert(p->GetRatingComplexity() >= -1);
    assert(p->GetRatingComplexity() <=  2);
    v[3] = p;
  }
  {
    const boost::shared_ptr<ribi::pvdb::Concept> p = Create("Concept with four Roman examples", { { "Example I/IV", pvdb::Competency::misc }, { "Example II/IV", pvdb::Competency::uninitialized }, { "Example III/IV", pvdb::Competency::profession }, { "Example III/IV", pvdb::Competency::social_surroundings } }, 1, 2, 0);
    assert(p);
    assert(p->GetRatingComplexity() >= -1);
    assert(p->GetRatingComplexity() <=  2);
    v[4] = p;
  }
  assert(std::count_if(v.begin(),v.end(),[](const boost::shared_ptr<ribi::pvdb::Concept>& p) { return !p; } ) == 0); //FIX 2012-01-02
  //assert(std::all_of(v.begin(),v.end(),[](const boost::shared_ptr<ribi::pvdb::Concept>& p) { return p; } ));
  assert(v[0]->GetExamples());

  return v;
  //Version below appears to work differently in cross compiler
  /*
  return
    {
      {
        Create("Concept without examples", ExamplesFactory::Create(), 0, 1, 2),
        Create("Concept with one example", { { "Only example", pvdb::Competency::profession } }, 1, 2, 0),
        Create("Concept with two examples", { { "First example", pvdb::Competency::organisations }, { "Second example", pvdb::Competency::social_surroundings } }, 2, 0, 1),
        Create("Concept with three examples", { { "Example 1 of 3", pvdb::Competency::target_audience }, { "Example 2 of 3", pvdb::Competency::ti_knowledge }, { "Example 3 of 3", pvdb::Competency::prof_growth } }, 0, 1, 2),
        Create("Concept with four Roman examples", { { "Example I/IV", pvdb::Competency::misc }, { "Example II/IV", pvdb::Competency::uninitialized }, { "Example III/IV", pvdb::Competency::profession }, { "Example III/IV", pvdb::Competency::social_surroundings } }, 1, 2, 0)
      }
    }; //Double braces not needed in future GCC version
  */
}
