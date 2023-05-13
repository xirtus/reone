/*
 * Copyright (c) 2020-2023 The reone project contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "reone/script/variable.h"

namespace reone {

namespace game {

struct RoutineContext;

namespace routine {

#define R_ROUTINE(x) script::Variable(x)(const std::vector<script::Variable> &args, const RoutineContext &ctx);

R_ROUTINE(unsupported)

R_ROUTINE(random)
R_ROUTINE(printString)
R_ROUTINE(floatToString)
R_ROUTINE(assignCommand)
R_ROUTINE(delayCommand)
R_ROUTINE(executeScript)
R_ROUTINE(clearAllActions)
R_ROUTINE(setFacing)
R_ROUTINE(switchPlayerCharacter)
R_ROUTINE(setPartyLeader)
R_ROUTINE(setAreaUnescapable)
R_ROUTINE(getAreaUnescapable)
R_ROUTINE(actionRandomWalk)
R_ROUTINE(actionMoveToLocation)
R_ROUTINE(actionMoveToObject)
R_ROUTINE(actionMoveAwayFromObject)
R_ROUTINE(getArea)
R_ROUTINE(getEnteringObject)
R_ROUTINE(getExitingObject)
R_ROUTINE(getPosition)
R_ROUTINE(getFacing)
R_ROUTINE(getItemPossessor)
R_ROUTINE(getItemPossessedBy)
R_ROUTINE(createItemOnObject)
R_ROUTINE(actionEquipItem)
R_ROUTINE(actionUnequipItem)
R_ROUTINE(getLastAttacker)
R_ROUTINE(actionAttack)
R_ROUTINE(getNearestCreature)
R_ROUTINE(actionPlayAnimation)
R_ROUTINE(getDistanceToObject)
R_ROUTINE(getIsObjectValid)
R_ROUTINE(actionOpenDoor)
R_ROUTINE(actionCloseDoor)
R_ROUTINE(playSound)
R_ROUTINE(getSpellTargetObject)
R_ROUTINE(actionCastSpellAtObject)
R_ROUTINE(getCurrentHitPoints)
R_ROUTINE(getMaxHitPoints)
R_ROUTINE(effectAssuredHit)
R_ROUTINE(getSubScreenID)
R_ROUTINE(cancelCombat)
R_ROUTINE(getCurrentForcePoints)
R_ROUTINE(getMaxForcePoints)
R_ROUTINE(pauseGame)
R_ROUTINE(setPlayerRestrictMode)
R_ROUTINE(getStringLength)
R_ROUTINE(getStringRight)
R_ROUTINE(getStringLeft)
R_ROUTINE(getSubString)
R_ROUTINE(findSubString)
R_ROUTINE(abs)
R_ROUTINE(effectHeal)
R_ROUTINE(effectDamage)
R_ROUTINE(effectAbilityIncrease)
R_ROUTINE(effectDamageResistance)
R_ROUTINE(effectResurrection)
R_ROUTINE(getPlayerRestrictMode)
R_ROUTINE(getCasterLevel)
R_ROUTINE(getFirstEffect)
R_ROUTINE(getNextEffect)
R_ROUTINE(removeEffect)
R_ROUTINE(getIsEffectValid)
R_ROUTINE(getEffectCreator)
R_ROUTINE(intToString)
R_ROUTINE(getFirstObjectInArea)
R_ROUTINE(getNextObjectInArea)
R_ROUTINE(d2)
R_ROUTINE(d3)
R_ROUTINE(d4)
R_ROUTINE(d6)
R_ROUTINE(d8)
R_ROUTINE(d10)
R_ROUTINE(d12)
R_ROUTINE(d20)
R_ROUTINE(d100)
R_ROUTINE(getMetaMagicFeat)
R_ROUTINE(getObjectType)
R_ROUTINE(getRacialType)
R_ROUTINE(fortitudeSave)
R_ROUTINE(reflexSave)
R_ROUTINE(willSave)
R_ROUTINE(getSpellSaveDC)
R_ROUTINE(effectACIncrease)
R_ROUTINE(effectSavingThrowIncrease)
R_ROUTINE(effectAttackIncrease)
R_ROUTINE(effectDamageIncrease)
R_ROUTINE(roundsToSeconds)
R_ROUTINE(hoursToSeconds)
R_ROUTINE(soundObjectSetFixedVariance)
R_ROUTINE(getGoodEvilValue)
R_ROUTINE(getPartyMemberCount)
R_ROUTINE(getAlignmentGoodEvil)
R_ROUTINE(getFirstObjectInShape)
R_ROUTINE(getNextObjectInShape)
R_ROUTINE(effectEntangle)
R_ROUTINE(signalEvent)
R_ROUTINE(eventUserDefined)
R_ROUTINE(effectDeath)
R_ROUTINE(actionGiveItem)
R_ROUTINE(actionTakeItem)
R_ROUTINE(vectorNormalize)
R_ROUTINE(getItemStackSize)
R_ROUTINE(getAbilityScore)
R_ROUTINE(getIsDead)
R_ROUTINE(vectorCreate)
R_ROUTINE(setFacingPoint)
R_ROUTINE(angleToVector)
R_ROUTINE(effectParalyze)
R_ROUTINE(setItemStackSize)
R_ROUTINE(getDistanceBetween)
R_ROUTINE(setReturnStrref)
R_ROUTINE(effectSleep)
R_ROUTINE(getItemInSlot)
R_ROUTINE(effectTemporaryForcePoints)
R_ROUTINE(effectChoke)
R_ROUTINE(setGlobalString)
R_ROUTINE(effectStunned)
R_ROUTINE(setCommandable)
R_ROUTINE(getCommandable)
R_ROUTINE(effectRegenerate)
R_ROUTINE(effectMovementSpeedIncrease)
R_ROUTINE(getHitDice)
R_ROUTINE(getTag)
R_ROUTINE(resistForce)
R_ROUTINE(getEffectType)
R_ROUTINE(getFactionEqual)
R_ROUTINE(setListening)
R_ROUTINE(setListenPattern)
R_ROUTINE(effectVisualEffect)
R_ROUTINE(getFactionAverageReputation)
R_ROUTINE(getGlobalString)
R_ROUTINE(getListenPatternNumber)
R_ROUTINE(actionJumpToObject)
R_ROUTINE(getWaypointByTag)
R_ROUTINE(effectLinkEffects)
R_ROUTINE(getObjectByTag)
R_ROUTINE(adjustAlignment)
R_ROUTINE(actionWait)
R_ROUTINE(actionStartConversation)
R_ROUTINE(actionPauseConversation)
R_ROUTINE(actionResumeConversation)
R_ROUTINE(effectBeam)
R_ROUTINE(getReputation)
R_ROUTINE(adjustReputation)
R_ROUTINE(getModuleFileName)
R_ROUTINE(effectForceResistanceIncrease)
R_ROUTINE(getLocation)
R_ROUTINE(actionJumpToLocation)
R_ROUTINE(location)
R_ROUTINE(applyEffectAtLocation)
R_ROUTINE(getIsPC)
R_ROUTINE(applyEffectToObject)
R_ROUTINE(speakString)
R_ROUTINE(getSpellTargetLocation)
R_ROUTINE(getPositionFromLocation)
R_ROUTINE(effectBodyFuel)
R_ROUTINE(getFacingFromLocation)
R_ROUTINE(getNearestObject)
R_ROUTINE(getNearestObjectByTag)
R_ROUTINE(intToFloat)
R_ROUTINE(floatToInt)
R_ROUTINE(stringToInt)
R_ROUTINE(getIsEnemy)
R_ROUTINE(getIsFriend)
R_ROUTINE(getIsNeutral)
R_ROUTINE(getPCSpeaker)
R_ROUTINE(getStringByStrRef)
R_ROUTINE(destroyObject)
R_ROUTINE(getModule)
R_ROUTINE(createObject)
R_ROUTINE(eventSpellCastAt)
R_ROUTINE(getLastSpellCaster)
R_ROUTINE(getLastSpell)
R_ROUTINE(getUserDefinedEventNumber)
R_ROUTINE(getSpellId)
R_ROUTINE(effectPoison)
R_ROUTINE(getLoadFromSaveGame)
R_ROUTINE(getName)
R_ROUTINE(getLastSpeaker)
R_ROUTINE(beginConversation)
R_ROUTINE(getLastPerceived)
R_ROUTINE(getLastPerceptionSeen)
R_ROUTINE(getLastClosedBy)
R_ROUTINE(getLastPerceptionVanished)
R_ROUTINE(getFirstInPersistentObject)
R_ROUTINE(getNextInPersistentObject)
R_ROUTINE(showLevelUpGUI)
R_ROUTINE(setItemNonEquippable)
R_ROUTINE(setButtonMashCheck)
R_ROUTINE(effectForcePushTargeted)
R_ROUTINE(giveItem)
R_ROUTINE(objectToString)
R_ROUTINE(effectImmunity)
R_ROUTINE(getIsImmune)
R_ROUTINE(effectDamageImmunityIncrease)
R_ROUTINE(getEncounterActive)
R_ROUTINE(setEncounterActive)
R_ROUTINE(getModuleItemAcquired)
R_ROUTINE(setCustomToken)
R_ROUTINE(getHasFeat)
R_ROUTINE(getHasSkill)
R_ROUTINE(actionUseSkill)
R_ROUTINE(getObjectSeen)
R_ROUTINE(getLastPlayerDied)
R_ROUTINE(actionDoCommand)
R_ROUTINE(getReflexAdjustedDamage)
R_ROUTINE(playAnimation)
R_ROUTINE(talentSpell)
R_ROUTINE(talentFeat)
R_ROUTINE(getHasSpellEffect)
R_ROUTINE(getEffectSpellId)
R_ROUTINE(getCreatureHasTalent)
R_ROUTINE(getCreatureTalentRandom)
R_ROUTINE(getCreatureTalentBest)
R_ROUTINE(actionUseTalentOnObject)
R_ROUTINE(jumpToLocation)
R_ROUTINE(effectTemporaryHitpoints)
R_ROUTINE(getSkillRank)
R_ROUTINE(getAttackTarget)
R_ROUTINE(getDistanceBetween2D)
R_ROUTINE(getIsInCombat)
R_ROUTINE(giveGoldToCreature)
R_ROUTINE(setIsDestroyable)
R_ROUTINE(setLocked)
R_ROUTINE(getLocked)
R_ROUTINE(getLastWeaponUsed)
R_ROUTINE(actionInteractObject)
R_ROUTINE(getLastUsedBy)
R_ROUTINE(getAbilityModifier)
R_ROUTINE(getDistanceToObject2D)
R_ROUTINE(getBlockingDoor)
R_ROUTINE(getIsDoorActionPossible)
R_ROUTINE(doDoorAction)
R_ROUTINE(getFirstItemInInventory)
R_ROUTINE(getNextItemInInventory)
R_ROUTINE(getClassByPosition)
R_ROUTINE(getLevelByPosition)
R_ROUTINE(getLevelByClass)
R_ROUTINE(getDamageDealtByType)
R_ROUTINE(getTotalDamageDealt)
R_ROUTINE(getLastDamager)
R_ROUTINE(effectSkillIncrease)
R_ROUTINE(getInventoryDisturbType)
R_ROUTINE(getInventoryDisturbItem)
R_ROUTINE(showUpgradeScreen)
R_ROUTINE(getGender)
R_ROUTINE(getIsTalentValid)
R_ROUTINE(getAttemptedAttackTarget)
R_ROUTINE(getTypeFromTalent)
R_ROUTINE(getIdFromTalent)
R_ROUTINE(playPazaak)
R_ROUTINE(getLastPazaakResult)
R_ROUTINE(displayFeedBackText)
R_ROUTINE(addJournalQuestEntry)
R_ROUTINE(removeJournalQuestEntry)
R_ROUTINE(getJournalEntry)
R_ROUTINE(playRumblePattern)
R_ROUTINE(stopRumblePattern)
R_ROUTINE(effectDamageForcePoints)
R_ROUTINE(effectHealForcePoints)
R_ROUTINE(getAttemptedSpellTarget)
R_ROUTINE(getLastOpenedBy)
R_ROUTINE(getHasSpell)
R_ROUTINE(openStore)
R_ROUTINE(actionSurrenderToEnemies)
R_ROUTINE(actionForceMoveToLocation)
R_ROUTINE(actionForceMoveToObject)
R_ROUTINE(jumpToObject)
R_ROUTINE(setMapPinEnabled)
R_ROUTINE(addMultiClass)
R_ROUTINE(getIsLinkImmune)
R_ROUTINE(effectDroidStun)
R_ROUTINE(effectForcePushed)
R_ROUTINE(giveXPToCreature)
R_ROUTINE(setXP)
R_ROUTINE(getXP)
R_ROUTINE(getBaseItemType)
R_ROUTINE(getItemHasItemProperty)
R_ROUTINE(actionEquipMostDamagingMelee)
R_ROUTINE(actionEquipMostDamagingRanged)
R_ROUTINE(effectForceResisted)
R_ROUTINE(getIsEncounterCreature)
R_ROUTINE(changeToStandardFaction)
R_ROUTINE(soundObjectPlay)
R_ROUTINE(soundObjectStop)
R_ROUTINE(soundObjectSetVolume)
R_ROUTINE(soundObjectSetPosition)
R_ROUTINE(getGold)
R_ROUTINE(effectForceFizzle)
R_ROUTINE(setLightsaberPowered)
R_ROUTINE(getLastSpellHarmful)
R_ROUTINE(musicBackgroundPlay)
R_ROUTINE(musicBackgroundStop)
R_ROUTINE(musicBackgroundChangeDay)
R_ROUTINE(musicBackgroundChangeNight)
R_ROUTINE(musicBattlePlay)
R_ROUTINE(musicBattleStop)
R_ROUTINE(ambientSoundPlay)
R_ROUTINE(ambientSoundStop)
R_ROUTINE(getLastKiller)
R_ROUTINE(getItemActivated)
R_ROUTINE(getItemActivator)
R_ROUTINE(getIsOpen)
R_ROUTINE(takeGoldFromCreature)
R_ROUTINE(getIsInConversation)
R_ROUTINE(effectAbilityDecrease)
R_ROUTINE(effectAttackDecrease)
R_ROUTINE(effectDamageDecrease)
R_ROUTINE(effectACDecrease)
R_ROUTINE(effectMovementSpeedDecrease)
R_ROUTINE(effectSavingThrowDecrease)
R_ROUTINE(getPlotFlag)
R_ROUTINE(setPlotFlag)
R_ROUTINE(effectInvisibility)
R_ROUTINE(effectForceShield)
R_ROUTINE(setDialogPlaceableCamera)
R_ROUTINE(getSoloMode)
R_ROUTINE(effectDisguise)
R_ROUTINE(effectTrueSeeing)
R_ROUTINE(setMaxStealthXP)
R_ROUTINE(effectBlasterDeflectionIncrease)
R_ROUTINE(effectHorrified)
R_ROUTINE(getCurrentStealthXP)
R_ROUTINE(getNumStackedItems)
R_ROUTINE(surrenderToEnemies)
R_ROUTINE(setCurrentStealthXP)
R_ROUTINE(getCreatureSize)
R_ROUTINE(awardStealthXP)
R_ROUTINE(setStealthXPEnabled)
R_ROUTINE(actionUnlockObject)
R_ROUTINE(actionLockObject)
R_ROUTINE(effectModifyAttacks)
R_ROUTINE(getAttemptedMovementTarget)
R_ROUTINE(getBlockingCreature)
R_ROUTINE(getChallengeRating)
R_ROUTINE(getFoundEnemyCreature)
R_ROUTINE(getSubRace)
R_ROUTINE(duplicateHeadAppearance)
R_ROUTINE(actionCastFakeSpellAtObject)
R_ROUTINE(actionCastFakeSpellAtLocation)
R_ROUTINE(cutsceneAttack)
R_ROUTINE(setLockOrientationInDialog)
R_ROUTINE(setLockHeadFollowInDialog)
R_ROUTINE(enableVideoEffect)
R_ROUTINE(startNewModule)
R_ROUTINE(disableVideoEffect)
R_ROUTINE(doSinglePlayerAutoSave)
R_ROUTINE(getGameDifficulty)
R_ROUTINE(getUserActionsPending)
R_ROUTINE(revealMap)
R_ROUTINE(showTutorialWindow)
R_ROUTINE(startCreditSequence)
R_ROUTINE(isCreditSequenceInProgress)
R_ROUTINE(getCurrentAction)
R_ROUTINE(getAppearanceType)
R_ROUTINE(getTrapBaseType)
R_ROUTINE(getFirstPC)
R_ROUTINE(getNextPC)
R_ROUTINE(setTrapDetectedBy)
R_ROUTINE(getIsTrapped)
R_ROUTINE(setEffectIcon)
R_ROUTINE(faceObjectAwayFromObject)
R_ROUTINE(getLastHostileActor)
R_ROUTINE(getModuleName)
R_ROUTINE(endGame)
R_ROUTINE(getRunScriptVar)
R_ROUTINE(getCreatureMovmentType)
R_ROUTINE(musicBackgroundGetBattleTrack)
R_ROUTINE(getHasInventory)
R_ROUTINE(addToParty)
R_ROUTINE(removeFromParty)
R_ROUTINE(addPartyMember)
R_ROUTINE(removePartyMember)
R_ROUTINE(isObjectPartyMember)
R_ROUTINE(getPartyMemberByIndex)
R_ROUTINE(getGlobalBoolean)
R_ROUTINE(setGlobalBoolean)
R_ROUTINE(getGlobalNumber)
R_ROUTINE(setGlobalNumber)
R_ROUTINE(aurPostString)
R_ROUTINE(barkString)
R_ROUTINE(effectPsychicStatic)
R_ROUTINE(playVisualAreaEffect)
R_ROUTINE(getLocalBoolean)
R_ROUTINE(setLocalBoolean)
R_ROUTINE(getLocalNumber)
R_ROUTINE(setLocalNumber)
R_ROUTINE(soundObjectGetPitchVariance)
R_ROUTINE(getGlobalLocation)
R_ROUTINE(setGlobalLocation)
R_ROUTINE(addAvailableNPCByObject)
R_ROUTINE(removeAvailableNPC)
R_ROUTINE(isAvailableCreature)
R_ROUTINE(addAvailableNPCByTemplate)
R_ROUTINE(spawnAvailableNPC)
R_ROUTINE(isNPCPartyMember)
R_ROUTINE(actionBarkString)
R_ROUTINE(getIsConversationActive)
R_ROUTINE(effectLightsaberThrow)
R_ROUTINE(effectWhirlWind)
R_ROUTINE(getPartyAIStyle)
R_ROUTINE(getNPCAIStyle)
R_ROUTINE(setNPCAIStyle)
R_ROUTINE(setNPCSelectability)
R_ROUTINE(getNPCSelectability)
R_ROUTINE(clearAllEffects)
R_ROUTINE(showPartySelectionGUI)
R_ROUTINE(getStandardFaction)
R_ROUTINE(givePlotXP)
R_ROUTINE(getMinOneHP)
R_ROUTINE(setMinOneHP)
R_ROUTINE(setGlobalFadeIn)
R_ROUTINE(setGlobalFadeOut)
R_ROUTINE(getLastHostileTarget)
R_ROUTINE(getLastAttackAction)
R_ROUTINE(getLastForcePowerUsed)
R_ROUTINE(getLastCombatFeatUsed)
R_ROUTINE(getLastAttackResult)
R_ROUTINE(getWasForcePowerSuccessful)
R_ROUTINE(actionFollowLeader)
R_ROUTINE(getIsDebilitated)
R_ROUTINE(playMovie)
R_ROUTINE(saveNPCState)
R_ROUTINE(getCategoryFromTalent)
R_ROUTINE(surrenderByFaction)
R_ROUTINE(changeFactionByFaction)
R_ROUTINE(playRoomAnimation)
R_ROUTINE(showGalaxyMap)
R_ROUTINE(setPlanetSelectable)
R_ROUTINE(setPlanetAvailable)
R_ROUTINE(getSelectedPlanet)
R_ROUTINE(soundObjectFadeAndStop)
R_ROUTINE(changeItemCost)
R_ROUTINE(getIsLiveContentAvailable)
R_ROUTINE(resetDialogState)
R_ROUTINE(getIsPoisoned)
R_ROUTINE(getSpellTarget)
R_ROUTINE(setSoloMode)
R_ROUTINE(effectCutSceneHorrified)
R_ROUTINE(effectCutSceneParalyze)
R_ROUTINE(effectCutSceneStunned)
R_ROUTINE(cancelPostDialogCharacterSwitch)
R_ROUTINE(setMaxHitPoints)
R_ROUTINE(noClicksFor)
R_ROUTINE(holdWorldFadeInForDialog)
R_ROUTINE(shipBuild)
R_ROUTINE(surrenderRetainBuffs)
R_ROUTINE(getCheatCode)
R_ROUTINE(setMusicVolume)
R_ROUTINE(createItemOnFloor)
R_ROUTINE(setAvailableNPCId)
R_ROUTINE(queueMovie)
R_ROUTINE(playMovieQueue)
R_ROUTINE(yavinHackCloseDoor)

// TSL

R_ROUTINE(getScriptParameter)
R_ROUTINE(setFadeUntilScript)
R_ROUTINE(effectForceBody)
R_ROUTINE(showChemicalUpgradeScreen)
R_ROUTINE(getSpellForcePointCost)
R_ROUTINE(effectFury)
R_ROUTINE(effectBlind)
R_ROUTINE(effectVPRegenModifier)
R_ROUTINE(effectCrush)
R_ROUTINE(getFeatAcquired)
R_ROUTINE(getSpellAcquired)
R_ROUTINE(showSwoopUpgradeScreen)
R_ROUTINE(grantFeat)
R_ROUTINE(grantSpell)
R_ROUTINE(spawnMine)
R_ROUTINE(setFakeCombatState)
R_ROUTINE(getOwnerDemolitionsSkill)
R_ROUTINE(setOrientOnClick)
R_ROUTINE(getInfluence)
R_ROUTINE(setInfluence)
R_ROUTINE(modifyInfluence)
R_ROUTINE(incrementGlobalNumber)
R_ROUTINE(decrementGlobalNumber)
R_ROUTINE(addBonusForcePoints)
R_ROUTINE(isStealthed)
R_ROUTINE(isMeditating)
R_ROUTINE(setHealTarget)
R_ROUTINE(getHealTarget)
R_ROUTINE(getRandomDestination)
R_ROUTINE(isFormActive)
R_ROUTINE(getSpellBaseForcePointCost)
R_ROUTINE(setKeepStealthInDialog)
R_ROUTINE(hasLineOfSight)
R_ROUTINE(forceHeartbeat)
R_ROUTINE(effectForceSight)
R_ROUTINE(isRunning)
R_ROUTINE(setForfeitConditions)
R_ROUTINE(getLastForfeitViolation)
R_ROUTINE(modifyReflexSavingThrowBase)
R_ROUTINE(modifyFortitudeSavingThrowBase)
R_ROUTINE(modifyWillSavingThrowBase)
R_ROUTINE(getScriptStringParameter)
R_ROUTINE(getObjectPersonalSpace)
R_ROUTINE(adjustCreatureAttributes)
R_ROUTINE(setCreatureAILevel)
R_ROUTINE(resetCreatureAILevel)
R_ROUTINE(addAvailablePUPByObject)
R_ROUTINE(assignPUP)
R_ROUTINE(spawnAvailablePUP)
R_ROUTINE(getPUPOwner)
R_ROUTINE(getIsPuppet)
R_ROUTINE(actionFollowOwner)
R_ROUTINE(getIsPartyLeader)
R_ROUTINE(getPartyLeader)
R_ROUTINE(removeNPCFromPartyToBase)
R_ROUTINE(creatureFlourishWeapon)
R_ROUTINE(effectMindTrick)
R_ROUTINE(effectFactionModifier)
R_ROUTINE(changeObjectAppearance)
R_ROUTINE(getIsXBox)
R_ROUTINE(effectDroidScramble)
R_ROUTINE(actionSwitchWeapons)
R_ROUTINE(playOverlayAnimation)
R_ROUTINE(unlockAllSongs)
R_ROUTINE(disableMap)
R_ROUTINE(detonateMine)
R_ROUTINE(disableHealthRegen)
R_ROUTINE(setCurrentForm)
R_ROUTINE(setDisableTransit)
R_ROUTINE(setInputClass)
R_ROUTINE(setForceAlwaysUpdate)
R_ROUTINE(displayMessageBox)
R_ROUTINE(displayDatapad)
R_ROUTINE(removeHeartbeat)
R_ROUTINE(removeEffectByID)
R_ROUTINE(removeEffectByExactMatch)
R_ROUTINE(adjustCreatureSkills)
R_ROUTINE(getSkillRankBase)
R_ROUTINE(enableRendering)
R_ROUTINE(getCombatActionsPending)
R_ROUTINE(getIsPlayerMadeCharacter)
R_ROUTINE(rebuildPartyTable)

// END TSL

namespace minigame {

R_ROUTINE(setLateralAccelerationPerSecond)
R_ROUTINE(setSpeedBlurEffect)
R_ROUTINE(playAnimation)
R_ROUTINE(adjustFollowerHitPoints)
R_ROUTINE(getLastFollowerHit)
R_ROUTINE(getLastObstacleHit)
R_ROUTINE(getObjectName)
R_ROUTINE(onDeath)
R_ROUTINE(setFollowerHitPoints)
R_ROUTINE(onDamage)
R_ROUTINE(removeAnimation)
R_ROUTINE(getPlayer)
R_ROUTINE(getEnemyCount)
R_ROUTINE(getEnemy)
R_ROUTINE(getHitPoints)
R_ROUTINE(getMaxHitPoints)
R_ROUTINE(getSphereRadius)
R_ROUTINE(getPosition)
R_ROUTINE(getPlayerOffset)
R_ROUTINE(getPlayerSpeed)
R_ROUTINE(getPlayerMinSpeed)
R_ROUTINE(getPlayerAccelerationPerSecond)
R_ROUTINE(setPlayerInvincibility)
R_ROUTINE(setPlayerSpeed)
R_ROUTINE(setPlayerMinSpeed)
R_ROUTINE(setPlayerAccelerationPerSecond)
R_ROUTINE(setPlayerTunnelPos)
R_ROUTINE(setPlayerTunnelNeg)
R_ROUTINE(getIsInvulnerable)
R_ROUTINE(startInvulnerability)
R_ROUTINE(setPlayerMaxSpeed)

// TSL

R_ROUTINE(getSwoopUpgrade)
R_ROUTINE(getTrackPosition)
R_ROUTINE(setFollowerPosition)
R_ROUTINE(destroyMiniGameObject)
R_ROUTINE(setJumpSpeed)
R_ROUTINE(playerApplyForce)

// END TSL

} // namespace minigame

} // namespace routine

} // namespace game

} // namespace reone