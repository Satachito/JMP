//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

import	UIKit
import	CoreData

class
JMPFetchedResultControllerVC: UITableViewController, NSFetchedResultsControllerDelegate {

	var	frc		:	NSFetchedResultsController! = nil {
		didSet	{
			frc.delegate = self
			frc.performFetch( nil )
		}
	}

	override	func
	numberOfSectionsInTableView( UITableView ) -> Int {
		return frc.sections!.count;
	}

	override	func
	tableView( UITableView, numberOfRowsInSection: Int ) -> Int {
		return frc.sections![ numberOfRowsInSection ].numberOfObjects;
	}

	func
	Update() {
		var wE : NSError? = nil
		if !frc.managedObjectContext.save( &wE ) {
			var wAC = UIAlertController(
				title			:	"Error"
			,	message			:	wE!.localizedDescription
			,	preferredStyle	:	.Alert
			)
			wAC.addAction(
				UIAlertAction(
					title:		"OK"
				,	style:		.Default
				) { _ in
					self.dismissViewControllerAnimated( true, completion:nil )
				}
			)
		}
	}

	override	func
	tableView( UITableView, canEditRowAtIndexPath: NSIndexPath ) -> Bool {
		return true
	}

	override	func
	tableView( UITableView, commitEditingStyle: UITableViewCellEditingStyle, forRowAtIndexPath:NSIndexPath ) {
		switch commitEditingStyle {
		case .Delete:
			frc.managedObjectContext.deleteObject( frc.objectAtIndexPath( forRowAtIndexPath ) as NSManagedObject )
			Update()
		default:
			break
		}
	}

	override	func
	tableView( UITableView, canMoveRowAtIndexPath:NSIndexPath ) -> Bool {
		return true;
	}

	func
	controllerWillChangeContent( NSFetchedResultsController! ) {
		self.tableView.beginUpdates()
	}

	func
	controller( NSFetchedResultsController
	,	didChangeSection	:	AnyObject!
	,	atIndex				:	Int
	,	forChangeType		:	NSFetchedResultsChangeType
	) {
		switch ( forChangeType ) {
		case .Insert:
			self.tableView.insertSections( NSIndexSet( index:atIndex ), withRowAnimation:.Fade )
		case .Delete:
			self.tableView.deleteSections( NSIndexSet( index:atIndex ), withRowAnimation:.Fade )
		default:
			break;
		}
	}

	func
	controller			(	 NSFetchedResultsController
	,	didChangeObject	:	AnyObject!
	,	atIndexPath		:	NSIndexPath
	,	forChangeType	:	NSFetchedResultsChangeType
	,	newIndexPath	:	NSIndexPath
	) {
		switch forChangeType {
		case .Insert:
			self.tableView.insertRowsAtIndexPaths( [ newIndexPath ], withRowAnimation:.Fade )
		case .Delete:
			self.tableView.deleteRowsAtIndexPaths( [  atIndexPath ], withRowAnimation:.Fade )
		case .Update:
			self.tableView.deleteRowsAtIndexPaths( [  atIndexPath ], withRowAnimation:.Fade )
			self.tableView.insertRowsAtIndexPaths( [  atIndexPath ], withRowAnimation:.Fade )
		case .Move:
			self.tableView.deleteRowsAtIndexPaths( [  atIndexPath ], withRowAnimation:.Fade )
			self.tableView.insertRowsAtIndexPaths( [ newIndexPath ], withRowAnimation:.Fade )
		default:
			break;
		}
	}

	func
	controllerDidChangeContent( NSFetchedResultsController ) {
		self.tableView.endUpdates()
	}
}
