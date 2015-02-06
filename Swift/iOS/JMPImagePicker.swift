import	UIKit
import	CoreMotion

//		CHECK 項目
//		各種デバイス
//		Rear, Front
//		Lock
//		各種回転
//		横向き対応

func
TempFiles() -> [ String ] {
	var	v = [ String ]()
	for w in NSFileManager.defaultManager().contentsOfDirectoryAtPath( NSTemporaryDirectory(), error: nil )! {
		v.append( NSTemporaryDirectory().stringByAppendingPathComponent( w as String ) )
	}
	return v
}

func
RemoveTempFies() {
	for w in TempFiles() {
		NSFileManager.defaultManager().removeItemAtPath( w, error: nil )
	}
}

func
SortedTempFiles() -> [ String ] {
	let v = sorted( TempFiles(), { return $0 < $1 } )
	return v
}

class
JMPImagePicker: UIImagePickerController, UIImagePickerControllerDelegate, UINavigationControllerDelegate {

	var	toBack		:	( UIViewController -> () ) = { p in }
	var	toAdvance	:	( UIViewController -> () ) = { p in }

	var	uMM		: CMMotionManager!

//vv	OUTLETS FOR IPC's OVERLAY VIEW

	@IBOutlet	weak	var	oPreviewIV		: UIImageView!
	@IBOutlet	weak	var	oFlipB			: UIView!
	@IBOutlet	weak	var	oFlashOnB		: UIButton!
	@IBOutlet	weak	var	oFlashOffB		: UIButton!
	@IBOutlet	weak	var	oFlashAutoB		: UIButton!
	@IBOutlet	weak	var	oCameraB		: UIButton!
	@IBOutlet	weak	var	oBackB			: UIButton!
	@IBOutlet	weak	var	oAdvanceB		: UIButton!
	@IBOutlet	weak	var	oThumbnailIV	: UIImageView!
	@IBOutlet	weak	var	oBadgeL			: UILabel!
//^^

	override func
	viewDidLoad() {
		super.viewDidLoad()
		self.delegate = self
		self.sourceType = .Camera
		self.cameraCaptureMode = .Photo
		self.showsCameraControls = false
		
		self.cameraOverlayView = NSBundle.mainBundle().loadNibNamed( "JMPImagePickerOverlay", owner:self, options:nil ).first as? UIView

		let	wH = self.view.bounds.size.height
		if wH <= 480 {	//	4S
			self.cameraOverlayView!.frame = self.view.bounds
			oPreviewIV.frame = self.view.bounds;
			oPreviewIV.contentMode = .ScaleAspectFill
		} else if wH <= 568 {	//	5, 5S
			self.cameraOverlayView!.frame = self.view.bounds
			self.cameraViewTransform = CGAffineTransformMakeTranslation( 0, 64 )
		} else if wH <= 667 {	//	6
			self.cameraOverlayView!.frame = CGRectMake( 0, 0, 375, 375 * 4 / 3 + 64 + 78 )
			self.cameraViewTransform = CGAffineTransformMakeTranslation( 0, 64 )
		}
//
	    uMM = CMMotionManager()
	    uMM.accelerometerUpdateInterval = 0.5	//	more than animation duration
	}

	func
	setCameraDevice( p: UIImagePickerControllerCameraDevice ) {
		super.cameraDevice = p
		let w = !UIImagePickerController.isFlashAvailableForCameraDevice( p )
		oFlashOnB.hidden = w
		oFlashOffB.hidden = w
		oFlashAutoB.hidden = w
	}

	func
	setCameraFlashMode( p: UIImagePickerControllerCameraFlashMode ) {
		super.cameraFlashMode = p
		oFlashOnB.hidden = true
		oFlashOffB.hidden = true
		oFlashAutoB.hidden = true
		switch p {
		case .On:	oFlashOnB.hidden = false
		case .Off:	oFlashOffB.hidden = false
		case .Auto:	oFlashAutoB.hidden = false
		}
	}
	
	func
	TempJPEGFileName() -> String {
		return NSTemporaryDirectory().stringByAppendingPathComponent( "\(NSDate().timeIntervalSinceReferenceDate).jpg" )
	}
	
	func
	SyncThumbnail() {
		let w = SortedTempFiles()
		self.oThumbnailIV.image = w.count > 0 ? Image( w.last! ) : nil
		oBadgeL.hidden = w.count == 0
		oBadgeL.text = "\(w.count)"
	}
	
	func
	imagePickerController(	//	Through Editing
		picker								: UIImagePickerController
	,	didFinishPickingMediaWithInfo info	: [ NSObject : AnyObject ]
	) {
		if let w = info[ UIImagePickerControllerOriginalImage ] as? UIImage {
		
			var	wImage: UIImage!
			
			switch picker.cameraDevice {
			case .Rear:
				oPreviewIV.image = UIImage( CGImage: w.CGImage, scale: 1, orientation: .Right )
				wImage = w
			case .Front:
				oPreviewIV.image = UIImage( CGImage: w.CGImage, scale: 1, orientation: .LeftMirrored )
				switch w.imageOrientation {
				case .Right:	wImage = UIImage( CGImage: w.CGImage, scale: 1, orientation: .LeftMirrored )	//	Portrait
				case .Left:		wImage = UIImage( CGImage: w.CGImage, scale: 1, orientation: .RightMirrored )	//	UpsideDown
				case .Up:		wImage = UIImage( CGImage: w.CGImage, scale: 1, orientation: .UpMirrored )		//	Right
				case .Down:		wImage = UIImage( CGImage: w.CGImage, scale: 1, orientation: .DownMirrored )	//	Left
				default:		break;
				}
			}

			UIImageJPEGRepresentation( wImage, 0.9 ).writeToFile( TempJPEGFileName(), atomically: true )
			SyncThumbnail()

			dispatch_after(
				dispatch_time( DISPATCH_TIME_NOW, Int64( NSEC_PER_SEC ) )
			,	dispatch_get_main_queue()
			,	{ self.oPreviewIV.image = nil }
			)
		}
	}

	@IBAction func
	DoCamera( p: AnyObject ) {
		takePicture()
	}
	
	@IBAction func
	DoFlip( p: AnyObject ) {
		self.cameraDevice = self.cameraDevice == .Rear ? .Front : .Rear
	}
	
	@IBAction func
	DoFlash( p: AnyObject ) {
		switch self.cameraFlashMode {
		case .Off:	self.cameraFlashMode = .On
		case .On:	self.cameraFlashMode = .Auto
		case .Auto:	self.cameraFlashMode = .Off
		}
	}
	
	@IBAction func
	DoBack( p: AnyObject ) {
		toBack( self )
	}
	
	@IBAction func
	DoAdvance( p: AnyObject ) {
		toAdvance( self )
	}
	
	@IBAction func
	DoPreview( p: AnyObject ) {
		self.navigationBarHidden = false
		let w = JMPImagePreviewVC( nibName:"JMPImagePreviewVC", bundle:nil )
		w.toBack = { p in
			self.popViewControllerAnimated( true )
			return
		}
		pushViewController( w, animated: true )
	}
	
	func
	navigationController( navigationController: UINavigationController, willShowViewController viewController: UIViewController, animated: Bool ) {
println( viewController )
println( self.viewControllers )

		self.cameraOverlayView!.hidden = !( viewController == self.viewControllers[ 0 ] as NSObject )
	}

	func
	navigationController( navigationController: UINavigationController, didShowViewController viewController: UIViewController, animated: Bool ) {
		SyncThumbnail()
	}

	override func
	viewWillAppear( p: Bool ) {
		RemoveTempFies()
		super.viewWillAppear( p )
		//	Using main queue is not recommended. So create new operation queue and pass it to startAccelerometerUpdatesToQueue.
		//	Dispatch U/I code to main thread using dispach_async in the handler.
		uMM.startAccelerometerUpdatesToQueue( NSOperationQueue() ) { p, _ in
			if p != nil {
				let w = abs( p.acceleration.y ) < abs( p.acceleration.x )
				?	p.acceleration.x > 0 ? M_PI * 3 / 2	:	M_PI / 2
				:	p.acceleration.y > 0 ? M_PI			:	0
				let	wAT = CGAffineTransformMakeRotation( CGFloat( w ) )
				dispatch_async(
					dispatch_get_main_queue()
				,	{	Animate() {
							self.oFlipB.transform = wAT
							self.oFlashOnB.transform = wAT
							self.oFlashOffB.transform = wAT
							self.oFlashAutoB.transform = wAT
							self.oCameraB.transform = wAT
							self.oBackB.transform = wAT
							self.oAdvanceB.transform = wAT
							self.oThumbnailIV.transform = wAT
							self.oBadgeL.transform = wAT
						}
					}
				)
			}
		}
	}

	override func
	viewDidDisappear( p: Bool ) {
		super.viewDidDisappear( p )
		uMM.stopAccelerometerUpdates()
		RemoveTempFies()
	}
}
























//	func
//	imagePickerController(
//		picker								: UIImagePickerController!
//	,	didFinishPickingImage	image		: UIImage!
//	,	editingInfo							: [ NSObject : AnyObject ]!
//	) {
//		println( "didFinishPickingImage" )
////		println( image )
////		println( editingInfo )
//	}
	

//	func
//	imagePickerControllerDidCancel( picker: UIImagePickerController ) {
//		println( "imagePickerControllerDidCancel" )
//	}
	







/*

didFinishPickingMediaWithInfo
[UIImagePickerControllerOriginalImage: <UIImage: 0x1702874e0> size {2448, 3264} orientation 3 scale 1.000000, UIImagePickerControllerMediaType: public.image, UIImagePickerControllerMediaMetadata: {
    DPIHeight = 72;
    DPIWidth = 72;
    Orientation = 6;
    "{Exif}" =     {
        ApertureValue = "2.27500704749987";
        BrightnessValue = "2.866315737262105";
        ColorSpace = 1;
        DateTimeDigitized = "2015:01:26 14:12:19";
        DateTimeOriginal = "2015:01:26 14:12:19";
        ExposureBiasValue = 0;
        ExposureMode = 0;
        ExposureProgram = 2;
        ExposureTime = "0.03333333333333333";
        FNumber = "2.2";
        Flash = 24;
        FocalLenIn35mmFilm = 29;
        FocalLength = "4.15";
        ISOSpeedRatings =         (
            100
        );
        LensMake = Apple;
        LensModel = "iPhone 6 back camera 4.15mm f/2.2";
        LensSpecification =         (
            "4.15",
            "4.15",
            "2.2",
            "2.2"
        );
        MeteringMode = 5;
        PixelXDimension = 3264;
        PixelYDimension = 2448;
        SceneType = 1;
        SensingMethod = 2;
        ShutterSpeedValue = "4.907640992149795";
        SubjectArea =         (
            1631,
            1223,
            1795,
            1077
        );
        SubsecTimeDigitized = 245;
        SubsecTimeOriginal = 245;
        WhiteBalance = 0;
    };
    "{MakerApple}" =     {
        1 = 2;
        14 = 0;
        2 = <39003900 39002a00 27002400 2700a401 51024c02 4d020c02 0c02d701 15024102 36003900 3d003600 3a003200 37004a01 8c025b02 52026a02 f801ba01 f9012b02 32003700 3c003800 38003500 3c001d01 5f026302 55025102 6702e501 ac01dc01 2b003300 34003700 37003300 3d00e600 08025502 6b025902 53027502 0302be01 23002e00 2b003200 35003100 3b00ac00 d6014702 5c027c02 78026502 35021802 1d002500 23002900 33002f00 35006300 9c015c02 30025c02 34022402 2e023302 19002000 1e002100 2d002e00 2f003b00 8d017002 4702f601 0c022002 32023c02 18002000 1f001e00 28002e00 2b003800 bf016802 e601e001 0f021d02 37024002 18002200 22001f00 26002f00 2e003400 d401dc01 e901df01 04022602 4302f501 1a002400 29002300 26002f00 33002d00 9401f601 0102e801 ec012a02 0202e501 1d002600 2e002700 29002d00 3a002800 22018701 1002e501 b901ca01 5c01e801 22002800 2f002c00 34003500 40002600 a500d300 0202d401 32019d00 27017701 28002900 2d002e00 3a003f00 44002600 55006a00 7c01b101 93006200 a1008000 2c002b00 2c003100 3d004300 40002400 31005c00 c5005101 67006d00 50003200 2f002d00 2e003400 3f004600 42002500 23005400 6a007400 6a007100 72004500 31003100 33003800 42004800 47002800 1d004a00 60006400 6e006f00 6a006b00>;
        3 =         {
            epoch = 0;
            flags = 1;
            timescale = 1000000000;
            value = 13097017102583;
        };
        4 = 1;
        5 = 239;
        6 = 234;
        7 = 1;
        8 =         (
            "-0.05619536",
            "-0.8392835",
            "-0.5431867"
        );
        9 = 275;
    };
    "{TIFF}" =     {
        DateTime = "2015:01:26 14:12:19";
        Make = Apple;
        Model = "iPhone 6";
        Software = "8.1.2";
        XResolution = 72;
        YResolution = 72;
    };
}]
*/


















//enum UIImagePickerControllerSourceType : Int {
//    
//    case PhotoLibrary
//    case Camera
//    case SavedPhotosAlbum
//}
//
//enum UIImagePickerControllerQualityType : Int {
//    
//    case TypeHigh // highest quality
//    case TypeMedium // medium quality, suitable for transmission via Wi-Fi 
//    case TypeLow // lowest quality, suitable for tranmission via cellular network
//    
//    case Type640x480 // VGA quality
//    
//    case TypeIFrame1280x720
//    case TypeIFrame960x540
//}
//
//enum UIImagePickerControllerCameraCaptureMode : Int {
//    
//    case Photo
//    case Video
//}
//
//enum UIImagePickerControllerCameraDevice : Int {
//    
//    case Rear
//    case Front
//}
//
//enum UIImagePickerControllerCameraFlashMode : Int {
//    
//    case Off
//    case Auto
//    case On
//}
//
