#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "multiapproximator.h"
#include "canvas.h"
#pragma GCC diagnostic pop

int main()
{
  typedef MultiApproximator<double,int> MultiApproximator_t;

  MultiApproximator_t a;
  a.Add(20.0, 3);
  a.Add(20.0, 7);
  a.Add(30.0,14);
  a.Add(30.0,16);
  a.Add(40.0, 2);
  a.Add(40.0, 8);
  a.Add(50.0,12);
  a.Add(50.0,18);

  const int max_x = 78;
  const int max_y = 20;
  Canvas c(max_x,max_y,Canvas::ColorSystem::invert,Canvas::CoordinatSystem::graph);

  c.DrawLine(0.0,0.0,max_x,0.0);
  c.DrawLine(0.0,0.0,0.0,max_y);

  //Plot values (note: these won't be visible in Canvas)
  {
    std::vector<double> xs;
    std::vector<double> ys;
    for (const auto p: a.GetContainer())
    {
      c.DrawDot(
        static_cast<double>(p.first),
        static_cast<double>(p.second)
      );
    }
  }

  //Plot approximation
  {
    for (int t=0; t != max_x; ++t)
    {
      try
      {
        const double x = static_cast<double>(t);
        const double y = a.Approximate(x);
        c.DrawDot(x,y);
      }
      catch (ExceptionNoExtrapolation<MultiApproximator_t::key_type>& e)
      {

      }
    }
  }
  std::cout << c << std::endl;
}

/* Screen output

TRACE '"Starting Approximator::Test"' line 144 in file '../../Classes/CppApproxi
mator/approximator.h': 'Starting Approximator::Test'
TRACE '"Completed Approximator::Test successfully"' line 155 in file '../../Clas
ses/CppApproximator/approximator.h': 'Completed Approximator::Test successfully'

 ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooood
oMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMooMMMMMMMMMMMMMMMMMd-oMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMo-dMMMMMMMMMMMMMMMdo--dMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMMdoodMMMMMMMMMMMMMddMdoodMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMMMdodMMMMMMMMMMMMddMMMdodMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMMMMMddMMMMMMMMMMdodMMMMMddMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMMMMMdoodMMMMMMdoodMMMMMMdoodMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMMMMMMdoodMMMMMdodMMMMMMMMdoodMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMMMMMMMMddMMMdodMMMMMMMMMMMMddMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMMMMMMMMMddMdoodMMMMMMMMMMMMMddMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMMMMMMMMMdodddMMMMMMMMMMMMMMMdodMMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMMMMMMMMMMd-oMMMMMMMMMMMMMMMMMd-oMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMMMMMMMMMMMooMMMMMMMMMMMMMMMMMMooMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
oMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
dMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
Press <RETURN> to close this window...

*/