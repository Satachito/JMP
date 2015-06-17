//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

import	UIKit
import	CoreData

class
JMPFetchedResultControllerVC: UITableViewController, NSFetchedResultsControllerDelegate {

	var	frc		:	NSFetchedResultsController! = nil {
		didSet	{
			frc.delegate = self
			try! frc.performFetch()
		}
	}
/*
	func
	Save() {
		do {
			try frc.managedObjectContext.save()
		} catch let error as NSError {
			let wAC = UIAlertController(
				title			:	"Error"
			,	message			:	error.localizedDescription
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
			//UC
		}
	}
*/
	override func
	viewDidDisappear( p: Bool ) {
		super.viewDidDisappear( p )
		try! frc.managedObjectContext.save()
	}

	override	func
	numberOfSectionsInTableView( UITableView ) -> Int {
        return frc.sections?.count ?? 0
	}

	override	func
	tableView( UITableView, numberOfRowsInSection: Int ) -> Int {
		return frc.sections![ numberOfRowsInSection ].numberOfObjects;
	}

	override	func
	tableView( UITableView, canEditRowAtIndexPath: NSIndexPath ) -> Bool {
		return true
	}

	override	func
	tableView( UITableView, canMoveRowAtIndexPath:NSIndexPath ) -> Bool {
		return true;
	}

	override	func
	tableView( UITableView, commitEditingStyle: UITableViewCellEditingStyle, forRowAtIndexPath:NSIndexPath ) {
		switch commitEditingStyle {
		case .Delete:
			frc.managedObjectContext.deleteObject( frc.objectAtIndexPath( forRowAtIndexPath ) as NSManagedObject )
			try! frc.managedObjectContext.save()
		default:
			break
		}
	}

	func
	controllerWillChangeContent( NSFetchedResultsController ) {
		tableView.beginUpdates()
	}

    func
	controller(				controller	: NSFetchedResultsController
	,	didChangeSection	sectionInfo	: NSFetchedResultsSectionInfo
	,	atIndex				sectionIndex: Int
	,	forChangeType		type		: NSFetchedResultsChangeType
	) {
		switch ( type ) {
		case .Insert:
			tableView.insertSections( NSIndexSet( index:sectionIndex ), withRowAnimation:.Fade )
		case .Delete:
			tableView.deleteSections( NSIndexSet( index:sectionIndex ), withRowAnimation:.Fade )
		default:
			break;
		}
	}

	func
	controller(				controller	: NSFetchedResultsController
	,	didChangeObject		anObject	: NSManagedObject
	,	atIndexPath			indexPath	: NSIndexPath?
	,	forChangeType		type		: NSFetchedResultsChangeType
	,						newIndexPath: NSIndexPath?
	) {
 		switch type {
		case .Insert:
			tableView.insertRowsAtIndexPaths( [ newIndexPath! ], withRowAnimation:.Fade )
		case .Delete:
			tableView.deleteRowsAtIndexPaths( [    indexPath! ], withRowAnimation:.Fade )
		case .Update:
			tableView.deleteRowsAtIndexPaths( [    indexPath! ], withRowAnimation:.Fade )
			tableView.insertRowsAtIndexPaths( [    indexPath! ], withRowAnimation:.Fade )
		case .Move:
			tableView.deleteRowsAtIndexPaths( [    indexPath! ], withRowAnimation:.Fade )
			tableView.insertRowsAtIndexPaths( [ newIndexPath! ], withRowAnimation:.Fade )
		}
	}

	func
	controllerDidChangeContent( NSFetchedResultsController ) {
		tableView.endUpdates()
	}
}
