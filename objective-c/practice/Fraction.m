// Program to work with fractions - class version

#import <Foundation/Foundation.h>

// ---- @interface section ----

@interface Fraction: NSObject
{
  int numerator;
  int denominator;
}

-(void)	print;
-(void) setNumerator: (int) n;
-(void) setDenominator: (int) d;
-(int) numerator;
-(int) denominator;

@end


// ---- @implementation section ----

@implementation Fraction
-(void) print
{
  NSLog(@"%i/%i", numerator, denominator);
}

-(void) setNumerator: (int) n
{
  numerator = n;
}

-(void) setDenominator: (int) d
{
  denominator = d;
}
-(int) numerator
{
  return numerator;
}
-(int) denominator
{
  return denominator;
}
@end

// ---- program section ----
int main(int argc, char* argv[]) 
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	Fraction *myFraction;

	// Create an instance of a Fraction
	myFraction = [Fraction alloc];
	myFraction = [myFraction init];

	// Set fraction to 1/3
	[myFraction setNumerator: 1]
	[myFraction setDenominator: 3];

	// Display the fraction using the print method
	NSLog (@"The value if myFraction is:");
	[myFraction print];
	[myFraction release];

	// Display the fraction data member
	NSLog("@The value of myFraction is: %i/%i",
		[myFraction numerator], [myFraction denominator]);
	[myFraction release];


	[pool drain];
	return 0;
}
