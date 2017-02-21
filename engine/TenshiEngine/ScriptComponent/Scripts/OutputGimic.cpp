#include "OutputGimic.h"

#include "OutputStartGimic.h"
#include "OutputConnectorOneCollGimic.h"
#include "OutputConnectorMultipleExporterGimic.h"
#include "OutputChangeEnableGimic.h"
#include "OutputConectorRandomGimic.h"
#include "OutputMoveObjectGimic.h"
# include "OutputChangePhysicesGimic.h"
#include "OutputConnectorStackGimic.h"
# include "OutputAnimation.h"
# include "OutputPlayAudio.h"
# include "OutputColliderActive.h"
#include "OutputConnectorToggleExporterGimic.h"
#include "OutputConnectorCooldownGimic.h"
#include "OutputConnectorCountGimick.h"
# include "ChangePlayerState.h"
#include "EnemyGate.h"
# include "OutPlayEnemyAnimation.h"
# include "OutputInstaanceObject.h"
# include "OutputChangeSceneGimick.h"
#include "OutputReisStartGimick.h"
# include "EventWave.h"
# include "OutputEventTimer.h"

OutputGimic * OutputGimic::GetOutputGimic(GameObject target)
{
	if (!target)return NULL;

	if (auto scr = target->GetScript<OutputStartGimic>())return scr;
	if (auto scr = target->GetScript<OutputConnectorOneCollGimic>())return scr;
	if (auto scr = target->GetScript<OutputConnectorMultipleExporterGimic>())return scr;
	if (auto scr = target->GetScript<OutputConnectorToggleExporterGimic>())return scr;
	if (auto scr = target->GetScript<OutputChangeEnableGimic>())return scr;
	if (auto scr = target->GetScript<OutputConectorRandomGimic>())return scr;
	if (auto scr = target->GetScript<OutputMoveObjectGimic>())return scr;
	if (auto scr = target->GetScript<OutputConnectorStackGimic>())return scr;
	if (auto scr = target->GetScript<OutputPlayAudio>())return scr;
	if (auto scr = target->GetScript<OutputColliderActive>())return scr;
	if (auto scr = target->GetScript<OutputConnectorCooldownGimic>())return scr;
	if (auto scr = target->GetScript<OutputChangeSceneGimick>())return scr;
	if (auto scr = target->GetScript<OutputReisStartGimick>())return scr;

	if (auto scr = target->GetScript<OutputChangePhysicesGimic>())return scr;
	if (auto scr = target->GetScript<OutputAnimation>())return scr;
	if (auto scr = target->GetScript<ChangePlayerState>())return scr;
	if (auto scr = target->GetScript<OutPlayEnemyAnimation>())return scr;
	if (auto scr = target->GetScript<OutputInstaanceObject>())return scr;
	if (auto scr = target->GetScript<EventWave>())return scr;
	if (auto scr = target->GetScript<OutputEventTimer>())return scr;
	
	

	return NULL;
}
