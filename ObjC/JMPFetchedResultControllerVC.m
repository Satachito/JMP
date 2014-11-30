//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#import "JMPFetchedResultControllerVC.h"

@interface
JMPFetchedResultControllerVC()<NSFetchedResultsControllerDelegate>
@end

@implementation
JMPFetchedResultControllerVC

-	(void)
setFrc:(NSFetchedResultsController*)p
{	_frc = p;
	_frc.delegate = self;
	[ _frc performFetch:nil ];
}


#pragma mark - Table View

-	(NSInteger)
numberOfSectionsInTableView:(UITableView*)pTV
{	return _frc.sections.count;
}

-	(NSInteger)
tableView:(UITableView*)pTV
numberOfRowsInSection:(NSInteger)p
{	return ((id<NSFetchedResultsSectionInfo>)_frc.sections[ p ]).numberOfObjects;
}

inline	static	void
Alert( NSObject* p )
{	[	[	UIAlertView.alloc
			initWithTitle:@"Error"
			message:p.description
			delegate:nil
			cancelButtonTitle:@"OK"
			otherButtonTitles:nil
		]
		show
	];
}

-	(void)
Update
{	NSError* wE = nil;
	if ( ! [ _frc.managedObjectContext save:&wE ] )
	{	assert( wE );
		Alert( @"ERROR" );
	}
}

-	(BOOL)
tableView:(UITableView*)pTV
canEditRowAtIndexPath:(NSIndexPath*)p
{	return YES;
}


-	(void)
tableView:(UITableView*)pTV
commitEditingStyle:(UITableViewCellEditingStyle)pEditingStyle
forRowAtIndexPath:(NSIndexPath*)p
{
    switch ( pEditingStyle )
	{
	case UITableViewCellEditingStyleDelete:
		[ _frc.managedObjectContext deleteObject:[ _frc objectAtIndexPath:p ] ];
 		[ self Update ];
		break;
	default:
		break;
	}
}

-	(BOOL)
tableView:(UITableView*)pTV
canMoveRowAtIndexPath:(NSIndexPath*)p
{	return YES;
}

-	(void)
controllerWillChangeContent:(NSFetchedResultsController*)pFRC
{	[ self.tableView beginUpdates ];
}

-	(void)
controller			:(NSFetchedResultsController*)		pFRC
didChangeSection	:(id <NSFetchedResultsSectionInfo>)	pSectionInfo
atIndex				:(NSUInteger)						p
forChangeType		:(NSFetchedResultsChangeType)		pType
{	switch ( pType )
	{
	case NSFetchedResultsChangeInsert:
		[ self.tableView insertSections:[ NSIndexSet indexSetWithIndex:p ] withRowAnimation:UITableViewRowAnimationFade ];
		break;
	case NSFetchedResultsChangeDelete:
		[ self.tableView deleteSections:[ NSIndexSet indexSetWithIndex:p ] withRowAnimation:UITableViewRowAnimationFade ];
		break;
    }
}

- (void)
controller		:(NSFetchedResultsController*)	pFRC
didChangeObject	:(id)							pObject
atIndexPath		:(NSIndexPath*)					pIndexPath
forChangeType	:(NSFetchedResultsChangeType)	pType
newIndexPath	:(NSIndexPath*)					pNewPath
{	switch ( pType )
	{
	case NSFetchedResultsChangeInsert:
		[ self.tableView insertRowsAtIndexPaths:@[   pNewPath ] withRowAnimation:UITableViewRowAnimationFade ];
		break;
	case NSFetchedResultsChangeDelete:
		[ self.tableView deleteRowsAtIndexPaths:@[ pIndexPath ] withRowAnimation:UITableViewRowAnimationFade ];
		break;
	case NSFetchedResultsChangeUpdate:
		[ self.tableView deleteRowsAtIndexPaths:@[ pIndexPath ] withRowAnimation:UITableViewRowAnimationFade ];
		[ self.tableView insertRowsAtIndexPaths:@[ pIndexPath ] withRowAnimation:UITableViewRowAnimationFade ];
		break;
	case NSFetchedResultsChangeMove:
		[ self.tableView deleteRowsAtIndexPaths:@[ pIndexPath ] withRowAnimation:UITableViewRowAnimationFade ];
		[ self.tableView insertRowsAtIndexPaths:@[   pNewPath ] withRowAnimation:UITableViewRowAnimationFade ];
		break;
    }
}

-	(void)
controllerDidChangeContent:(NSFetchedResultsController*)pFRC
{	[ self.tableView endUpdates ];
}

/*
// Implementing the above methods to update the table view in response to individual changes may have performance implications if a large number of changes are made simultaneously. If this proves to be an issue, you can instead just implement controllerDidChangeContent: which notifies the delegate that all section and object changes have been processed. 
 
 - (void)controllerDidChangeContent:(NSFetchedResultsController *)controller
{
    // In the simplest, most efficient, case, reload the table view.
    [self.tableView reloadData];
}
 */
@end
