//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#import	<UIKit/UIKit.h>

@interface
JMPSlider : UISlider

@property	(nonatomic, strong)	void	(^slided)( UISlider* pS, float p );
@end

