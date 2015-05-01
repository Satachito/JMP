import UIKit

class
JMPHorizontalLayoutV: UIView {

	override func
	layoutSubviews() {
		super.layoutSubviews()

		if subviews.count == 0 { return }

		var	wTotalSize = 0 as CGFloat
		for wV in subviews {
			wTotalSize += wV.frame.size.width
		}
		var	wMargin = ( bounds.size.width - wTotalSize ) / 2 / CGFloat( subviews.count )
		var	wExtentTotal = 0 as CGFloat
		for i in 0 ..< subviews.count {
			let	wV = subviews[ i ] as! UIView
			wV.center = CGPointMake(
				wV.frame.size.width / 2 + wExtentTotal + wMargin
			,	wV.center.y
			)
			wExtentTotal += wV.frame.size.width + wMargin * 2
		}
	}
}
