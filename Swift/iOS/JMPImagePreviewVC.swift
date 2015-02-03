import	UIKit

class
JMPImagePreviewVC: UIViewController {
	
	var	toBack		:	( UIViewController -> () ) = { p in }
	var	toAdvance	:	( UIViewController -> () ) = { p in }

	@IBOutlet	weak	var	oIV		: UIImageView!
	@IBOutlet	weak	var	oSV		: UIScrollView!

	var	u		= [ ( String, UIView, UIImageView, UIView ) ]()	//	File, TrayV, ImageV, IndicatorV
	var	uIndex	: Int = 0 {
		didSet {

			for ( _, _, imageV, indicatorV ) in u {
				imageV.alpha = 0.7
				indicatorV.backgroundColor = UIColor.clearColor()
			}

			let ( _, trayV, imageV, indicatorV ) = u[ uIndex ]

			imageV.alpha = 1

			oSV.scrollRectToVisible( trayV.frame, animated: true )
			
			indicatorV.backgroundColor = UIColor( red: 204.0 / 255.0, green: 169.0 / 255.0, blue: 51.0 / 255.0, alpha: 1 )

			oIV.image = imageV.image
			
			TransitAnimation( uIndex < oldValue ? kCATransitionFromLeft : kCATransitionFromRight )
		}
	}

	func	//	Must be called inside the context where uIndex != nil
	TransitAnimation( subtype: String ) {
		let	wT = CATransition()
		wT.duration = 0.3
		wT.type = kCATransitionPush
		wT.subtype = subtype
		wT.timingFunction = CAMediaTimingFunction( name: kCAMediaTimingFunctionEaseInEaseOut )

		oIV.layer.addAnimation( wT, forKey:"" )
	}
	
	func
	Sync() {
		for ( _, trayV, imageV, _ ) in u { trayV.removeFromSuperview() }
		u.removeAll()
		for wPath in SortedTempFiles() {
			if let wImage = Image( wPath ) {
				let wImageV = UIImageView( frame:CGRectMake( 0, 0, 50, 50 ) )
				wImageV.contentMode = .ScaleAspectFill
				wImageV.clipsToBounds = true
				wImageV.image = wImage
				
				let	wIndicatorV = UIView( frame:CGRectMake( 0, 53, 50, 3 ) )
				
				let	wV = UIView( frame: CGRectMake( CGFloat( u.count * 50 ), 0, 50, 56 ) )
				wV.addSubview( wImageV )
				wV.addSubview( wIndicatorV )
				
				oSV.addSubview( wV )
				u.append( ( wPath, wV, wImageV, wIndicatorV ) )
			}
		}
		oSV.contentSize = CGSizeMake( CGFloat( u.count * 50 ), 56 )
	}
	
	@IBAction func
	DoTap( p: UITapGestureRecognizer ) {
		uIndex = Int( p.locationInView( oSV ).x / 50 )
	}
	
	override func
	viewWillAppear( p: Bool ) {
		super.viewWillAppear( p )
		Sync()
		if u.count > 0 { uIndex = 0 }
		
		self.navigationController!.navigationBar.barTintColor = UIColor( red: 204.0 / 255.0, green: 169.0 / 255.0, blue: 51.0 / 255.0, alpha: 1 )
		self.navigationItem.title = "確認"
		self.navigationItem.rightBarButtonItem = UIBarButtonItem( title: "進む", style: .Plain, target: self, action: "DoAdvance:" )
		self.navigationItem.rightBarButtonItem?.tintColor = UIColor.blackColor()
		self.navigationController!.navigationBar.tintColor = UIColor.blackColor()
	}

	@IBAction func
	DoSwipeRight( p: UISwipeGestureRecognizer! ) {
		if uIndex > 0 { uIndex-- }
	}
	
	@IBAction func
	DoSwipeLeft( p: UISwipeGestureRecognizer! ) {
		if uIndex + 1 < u.count { uIndex++ }
	}
	
	@IBAction func
	DoDelete( p: AnyObject! ) {
		let	( path, _, _, _ ) = u[ uIndex ]
		NSFileManager.defaultManager().removeItemAtPath( path, error: nil )
		if u.count == 1 {
			toBack( self )
		} else {
			Sync()
			uIndex = uIndex >= u.count ? u.count - 1 : uIndex
		}
	}
	@IBAction func
	DoAdvance( p: AnyObject! ) {
		toAdvance( self )
	}
	
	override func
	viewDidLoad() {
		super.viewDidLoad()
	}
}
