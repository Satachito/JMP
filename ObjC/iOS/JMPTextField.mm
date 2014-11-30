//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#import "JMPTextField.h"

@implementation
JMPTextField

-	(void)
setBorderColor:(UIColor*)p
{	self.layer.borderColor = p.CGColor;
}

-	(UIColor*)
borderColor
{	return [ UIColor colorWithCGColor:self.layer.borderColor ];
}
@end

