/*--------------------------------------------------------------------*/
/* checkerDT.c                                                        */
/* Author:                                                            */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "checkerDT.h"
#include "dynarray.h"
#include "path.h"



/* see checkerDT.h for specification */
boolean CheckerDT_Node_isValid(Node_T oNNode) {
   Node_T oNParent;
   Path_T oPNPath;
   Path_T oPPPath;

   /* Sample check: a NULL pointer is not a valid node */
   if(oNNode == NULL) {
      fprintf(stderr, "A node is a NULL pointer\n");
      return FALSE;
   }
   
   /* Check if Path is NULL */
   oPNPath = Node_getPath(oNNode);
   if(oPNPath == NULL) {
       fprintf(stderr, "A node's path is NULL\n");
       return FALSE;
   }

   oNParent = Node_getParent(oNNode);
   if(oNParent != NULL) {
      oPPPath = Node_getPath(oNParent);

      /* Check if parent depth is one less than child depth */
      if(Path_getDepth(oPPPath) != Path_getDepth(oPNPath) - 1) {
         fprintf(stderr, "Parent depth is not one less than child depth: (%s) (%s)\n", 
               Path_getPathname(oPPPath), Path_getPathname(oPNPath));
         return FALSE;
      }

      /* Check if parent path is the exact prefix of the child path */
      if(Path_getSharedPrefixDepth(oPNPath, oPPPath) !=
         Path_getDepth(oPNPath) - 1) {
         fprintf(stderr, "P-C nodes don't have P-C paths: (%s) (%s)\n",
                 Path_getPathname(oPPPath), Path_getPathname(oPNPath));
         return FALSE;
      }
   } else {
      /* If there is no parent this must be a root node */
      if(Path_getDepth(oPNPath) != 1) {
          fprintf(stderr, "Node has no parent, but its path depth is not 1: (%s)\n", 
                  Path_getPathname(oPNPath));
          return FALSE;
      }
   }

   return TRUE;
}

/* Performs a pre-order traversal of the tree rooted at oNNode, and
updates the total number of nodes traversed in location pointed to by
pulNodeCount. Returns FALSE if a broken invariant is found and returns 
TRUE otherwise.
*/
static boolean CheckerDT_treeCheck(Node_T oNNode, size_t *pulNodeCount) {
   size_t ulIndex;

   if(oNNode!= NULL) {
      /* Count this node */
      (*pulNodeCount)++;

      /* Check if node is valid */
      if(!CheckerDT_Node_isValid(oNNode))
         return FALSE;

      /* Recur on every child of oNNode */
      for(ulIndex = 0; ulIndex < Node_getNumChildren(oNNode); ulIndex++)
      {
         Node_T oNChild = NULL;
         int iStatus = Node_getChild(oNNode, ulIndex, &oNChild);
         
         /* Check number of children */
         if(iStatus != SUCCESS) {
            fprintf(stderr, "getNumChildren claims more children than getChild returns\n");
            return FALSE;
         }

         /* Check if correct parent pointer */
         if (oNNode != Node_getParent(oNChild)) {
             fprintf(stderr, "Child's parent pointer does not match actual parent\n");
             return FALSE;
         }

         /* Check proper children ordering */
         if (ulIndex > 0) {
            Node_T oNPreviousChild = NULL;
            Node_getChild(oNNode, ulIndex - 1, &oNPreviousChild);
             
            if (Path_comparePath(Node_getPath(oNPreviousChild), Node_getPath(oNChild)) >= 0) {
               fprintf(stderr, "Children array not in proper compare order\n");
               return FALSE;
            }
         }

         /* if recurring down one subtree results in a failed check
            farther down, passes the failure back up immediately */
         if(!CheckerDT_treeCheck(oNChild, pulNodeCount))
            return FALSE;
      }
   }
   return TRUE;
}

/* see checkerDT.h for specification */
boolean CheckerDT_isValid(boolean bIsInitialized, Node_T oNRoot,
                          size_t ulCount) {
   size_t ulCurrentNodeCount = 0;
  
   /* Initialization State Check */
   if(!bIsInitialized) {
      if(ulCount != 0) {
         fprintf(stderr, "Not initialized, but count is not 0\n");
         return FALSE;
      }

      if (oNRoot != NULL) {
         fprintf(stderr, "Not initialized, but root is not NULL\n");
         return FALSE;
      }

      return TRUE;
   }

   /* Empty Tree Check */
   if (ulCount == 0) {
      if (oNRoot != NULL) {
         fprintf(stderr, "Count is zero, but root is not NULL\n");
         return FALSE;
      }
   }

   /* Non-Empty Tree Check */
   if (ulCount != 0) {
      if (oNRoot == NULL) {
         fprintf(stderr, "Count is non-zero, but root is NULL\n");
         return FALSE;
      }

      if (Node_getParent(oNRoot) != NULL) {
         fprintf(stderr, "Parent of root node not NULL\n");
         return FALSE;
      }
   }

   /* Recursively Traverse the Tree */
   if (!CheckerDT_treeCheck(oNRoot, &ulCurrentNodeCount)) {
      return FALSE; 
   }

   /* Check Correct Node Count*/
   if (ulCurrentNodeCount != ulCount) {
      fprintf(stderr, "Real node count (%lu) does not match ulCount (%lu)\n", ulCurrentNodeCount, ulCount);
      return FALSE;
   }

   return TRUE;
}
