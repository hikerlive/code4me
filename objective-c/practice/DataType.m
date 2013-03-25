#import <Foundation/Foundation.h>

int main(int argc, char* argv[])
{
  NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

  int		integerVar = 100;
  float		floatingVar = 331.79;
  double	doubleVar = 8.44e+11;
  char		charVar = 'W';

  NSLog(@"integerVar = %i", intergerVar);
  NSLog(@"floatingVar = %f", floatingVar);
  NSLog(@"doubleVar= %e", doubleVar);
  NSLog(@"doubleVar= %g", doubleVar);
  NSLog(@"charVar= %c", charVar);

  [pool drain];
  return 0;
}

// Output
integerVar = 100
floatingVar = 333.790009
doubleVar = 8.440000e+11
doubleVar = 8.44e+11
charVar = W
