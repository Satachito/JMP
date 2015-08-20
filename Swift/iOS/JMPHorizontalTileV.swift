import UIKit

class
JMPHorizontalTileV: UIView {

	override func
	layoutSubviews() {
		super.layoutSubviews()

		if subviews.count == 0 { return }

		let	wWidth = bounds.size.width / CGFloat( subviews.count )
		for i in 0 ..< subviews.count {
			let	wV = subviews[ i ] as UIView
			wV.frame = CGRectMake( wWidth * CGFloat( i ), 0, wWidth, bounds.size.height )
		}
	}
}

