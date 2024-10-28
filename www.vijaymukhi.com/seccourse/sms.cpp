#include <txtrich.h>
#include <smscmds.h>
#include <eikenv.h> 
#include <smuthdr.h>
#include <mtclbase.h>
#include <smsclnt.h>
#include <cmdabsorber.h>
#include <mtclreg.h> 
#include <eikapp.h>  
#include <eikdoc.h>  
#include <eikappui.h> 
#include <ckninfo.h>
const TUid KUidGDSMS = { 0x101F3CD9 };  
class CGDSMSAppView : public CCoeControl
{
public:
CGDSMSAppView(const TRect& aRect);
};
class CGDSMSAppUi : public CEikAppUi, public MMsvSessionObserver
{
public:
void ConstructL();
void HandleCommandL(TInt aCommand);
void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
TBuf<128> aa;
TMsvId iMsvId; 
CMsvSession* iSession; 
CBaseMtm* iMtm;
CClientMtmRegistry* iMtmReg;
};
class CGDSMSDocument : public CEikDocument
{
public:
CEikAppUi* CreateAppUiL();
CGDSMSDocument(CEikApplication& aApp);
};
class CGDSMSApplication : public CEikApplication
{
public:
TUid AppDllUid() const;
CApaDocument* CreateDocumentL();
};
CGDSMSAppView::CGDSMSAppView(const TRect& aRect)
{
CreateWindowL();
SetRect(aRect);
ActivateL();
}
void CGDSMSAppUi::ConstructL()
{
BaseConstructL();   
new CGDSMSAppView(ClientRect()); 
iSession = CMsvSession::OpenSyncL(*this);
iMtmReg = CClientMtmRegistry::NewL(*iSession);
}
void CGDSMSAppUi::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* /*aArg3*/)
{
}
void CGDSMSAppUi::HandleCommandL(TInt aCommand)
{
if ( aCommand == 100)
{
TMsvEntry newEntry;
newEntry.iMtm = KUidMsgTypeSMS;  
newEntry.iType = KUidMsvMessageEntry;
newEntry.iServiceId = KMsvLocalServiceIndexEntryId;  
CMsvEntry* entry = CMsvEntry::NewL(*iSession, KMsvDraftEntryIdValue ,TMsvSelectionOrdering());
CMsvOperationWait* wait = CMsvOperationWait::NewLC();
wait->Start();
CMsvOperation* oper = entry->CreateL(newEntry,wait->iStatus);
CActiveScheduler::Start();
TMsvLocalOperationProgress progress = McliUtils::GetLocalProgressL(*oper);
aa.Format(_L("id=%d"),progress.iId);
CCknInfoDialog::RunDlgLD(_L("hi1"),aa);
entry->SetEntryL(progress.iId);
iMsvId = entry->EntryId();
iMtm = iMtmReg->NewMtmL(entry->Entry().iMtm);
iMtm->SetCurrentEntryL(entry);  
TMsvEntry msvEntry = iMtm->Entry().Entry();
CRichText& mtmBody = iMtm->Body();
mtmBody.InsertL(0, _L("Vijay2 Mukhi and Sonal")); 
CSmsClientMtm* smsMtm = STATIC_CAST(CSmsClientMtm*, iMtm);
CSmsHeader& header = smsMtm->SmsHeader();
header.SetFromAddressL(_L("919821896397"));
CSmsSettings* serviceSettings = &(smsMtm->ServiceSettings());
CSmsNumber* sc = &(serviceSettings->SCAddress(serviceSettings->DefaultSC()));
header.Message().SetServiceCenterAddressL(sc->Address());
smsMtm->AddAddresseeL(_L("9820889366"),msvEntry.iDetails);
smsMtm->SaveMessageL();  
CMsvEntry* parentEntry = CMsvEntry::NewL(iMtm->Session(), msvEntry.Parent(),TMsvSelectionOrdering());
wait->Start();
parentEntry->MoveL(iMsvId, KMsvGlobalOutBoxIndexEntryId, wait->iStatus);
CActiveScheduler::Start();   
CMsvEntrySelection* selection = new CMsvEntrySelection;
selection->AppendL(iMsvId);  
TBuf8<1> dummyParams;
wait->Start();
iMtm->InvokeAsyncFunctionL(ESmsMtmCommandScheduleCopy,*selection,dummyParams,wait->iStatus);
CActiveScheduler::Start();
CleanupStack::PopAndDestroy(1);
}
if ( aCommand == EEikCmdExit)
Exit();
}
CGDSMSDocument::CGDSMSDocument(CEikApplication& aApp): CEikDocument(aApp)
{
}
CEikAppUi* CGDSMSDocument::CreateAppUiL()
{
return (new CGDSMSAppUi);
}
TUid CGDSMSApplication::AppDllUid() const
{
return KUidGDSMS;
}
CApaDocument* CGDSMSApplication::CreateDocumentL()
{
return (new CGDSMSDocument(*this));
}
EXPORT_C CApaApplication* NewApplication()
{
return (new CGDSMSApplication);
}
GLDEF_C TInt E32Dll(TDllReason)
{
return KErrNone;
}
