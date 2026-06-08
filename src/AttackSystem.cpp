#include "AttackSystem.h"
#include "Pawns.h"
#include "Pawn.h"
#include "Board.h"
#include "Hex.h"

#include <cmath>
#include <iostream>

namespace {
    constexpr float kLungeFraction = 0.05f;
    constexpr float kLungeSpeed = 680.0f;
    constexpr float kProjectileSpeed = 450.0f;
    constexpr float kProjectileScale = 0.07f;
    constexpr float kRadToDeg = 180.0f / 3.14159265f;
    constexpr float kSpriteForwardOffset = 90.0f;
}

AttackSystem::AttackSystem(Pawns* pawns)
    : pawns(pawns)
{
    texturesLoaded = arrowTexture.loadFromFile("assets/equipment/arrow.png")
        && rockTexture.loadFromFile("assets/equipment/rock.png");
}

void AttackSystem::attack(int pawnNum, int attackedNum, Equipment* weapon)
{
    Pawn* attacker = pawns->pawnDict[pawnNum];
    if (!hasEnoughActions(attacker, weapon)) {
        std::cout << "Not enough actions\n";
        return;
    }

    Pawn* attacked = pawns->pawnDict[attackedNum];
    const int attackValue = calculateAttackValue(attacker, weapon);

    if (weapon->isRanged()) {
        const int missMax = attacker->getMissMax(weapon->getName());
        startRangedAttack(pawnNum, attackedNum, weapon, attackValue, missMax);
    }
    else {
        resolveMeleeAttack(attacker, attacked, weapon, attackValue);
        if (!attacked->isAlive()) {
            pawns->death(attacked);
        }
        attacker->reduceActions(weapon->getAttackActions());
        pawns->resetTurn();
    }
}

bool AttackSystem::hasEnoughActions(Pawn* attacker, Equipment* weapon) const
{
    return weapon->getAttackActions() <= attacker->getRemainingActions();
}

int AttackSystem::calculateAttackValue(Pawn* attacker, Equipment* weapon) const
{
    int attackValue = weapon->getAttackValue();
    if (attacker->hasItem("gauntlets")) {
        attackValue++;
    }
    return attackValue;
}

void AttackSystem::resolveMeleeAttack(Pawn* attacker, Pawn* attacked,
    Equipment* weapon, int attackValue)
{
    attacked->attack(attackValue);
    if (weapon->getName() == "dagger" || weapon->getName() == "long dagger") {
        attacker->removeEquipment(weapon);
    }
}

float AttackSystem::facingAngleToward(sf::Vector2f from, sf::Vector2f to) const
{
    const sf::Vector2f delta = to - from;
    return std::atan2(delta.y, delta.x) * kRadToDeg + kSpriteForwardOffset;
}

sf::Vector2f AttackSystem::pawnScreenPosition(Pawn* pawn) const
{
    const sf::Vector2f origin = pawns->board->hexDict.at(pawn->getHexCoords())->getOrigin();
    return sf::Vector2f(origin.x, origin.y);
}

bool AttackSystem::isSlingshot(const std::string& weaponName) const
{
    return weaponName == "slingshot" || weaponName == "large slingshot";
}

const sf::Texture& AttackSystem::projectileTextureFor(const std::string& weaponName) const
{
    if (isSlingshot(weaponName)) {
        return rockTexture;
    }
    return arrowTexture;
}

void AttackSystem::startRangedAttack(int pawnNum, int attackedNum, Equipment* weapon,
    int attackValue, int missMax)
{
    Pawn* attacker = pawns->pawnDict[pawnNum];
    Pawn* attacked = pawns->pawnDict[attackedNum];

    if (!texturesLoaded) {
        attacked->rangedAttack(attackValue, missMax);
        if (!attacked->isAlive()) {
            pawns->death(attacked);
        }
        attacker->reduceActions(weapon->getAttackActions());
        pawns->resetTurn();
        return;
    }
    const sf::Vector2f homePos = pawnScreenPosition(attacker);
    const sf::Vector2f targetPos = pawnScreenPosition(attacked);
    const sf::Vector2f lungePos = homePos + (targetPos - homePos) * kLungeFraction;

    activeRangedAttack = PendingRangedAttack{
        pawnNum, attackedNum, weapon, attackValue, missMax,
        homePos, lungePos, RangedAttackPhase::LungeOut, false, nullptr
    };

    attacker->setAttackFacing(facingAngleToward(homePos, targetPos));
    startLungeOut(attacker, attacked, homePos);
}

void AttackSystem::startLungeOut(Pawn* attacker, Pawn* attacked, sf::Vector2f homePos)
{
    const sf::Vector2f targetPos = pawnScreenPosition(attacked);
    const sf::Vector2f lungePos = homePos + (targetPos - homePos) * kLungeFraction;
    attacker->startAttackLunge({ homePos, lungePos }, kLungeSpeed);
}

void AttackSystem::startLungeIn(Pawn* attacker, sf::Vector2f lungePos, sf::Vector2f homePos)
{
    attacker->startAttackLunge({ lungePos, homePos }, kLungeSpeed);
}

void AttackSystem::spawnProjectile(sf::Vector2f startPos, sf::Vector2f targetPos,
    const std::string& weaponName)
{
    if (!texturesLoaded || !activeRangedAttack) {
        return;
    }
    activeRangedAttack->projectile = std::make_unique<Projectile>(
        projectileTextureFor(weaponName), startPos, targetPos,
        kProjectileSpeed, kProjectileScale);
}

void AttackSystem::resolveRangedImpact()
{
    if (!activeRangedAttack || activeRangedAttack->impactResolved) {
        return;
    }

    Pawn* attacker = pawns->pawnDict[activeRangedAttack->attackerNum];
    Pawn* attacked = pawns->pawnDict[activeRangedAttack->attackedNum];
    Equipment* weapon = activeRangedAttack->weapon;

    attacked->rangedAttack(activeRangedAttack->attackValue, activeRangedAttack->missMax);

    if (!attacked->isAlive()) {
        pawns->death(attacked);
    }

    attacker->reduceActions(weapon->getAttackActions());
    pawns->deferredHighlightRefresh = true;
    activeRangedAttack->impactResolved = true;
}

void AttackSystem::update(float dt)
{
    if (!activeRangedAttack) {
        return;
    }

    PendingRangedAttack& pending = *activeRangedAttack;
    Pawn* attacker = pawns->pawnDict[pending.attackerNum];

    if (pending.phase == RangedAttackPhase::LungeOut) {
        if (!attacker->isAttackLunging()) {
            spawnProjectile(pending.lungePos, pawnScreenPosition(pawns->pawnDict[pending.attackedNum]),
                pending.weapon->getName());
            startLungeIn(attacker, pending.lungePos, pending.homePos);
            pending.phase = RangedAttackPhase::LungeInAndProjectile;
        }
    }
    else if (pending.phase == RangedAttackPhase::LungeInAndProjectile) {
        if (pending.projectile) {
            pending.projectile->update(dt);
            if (pending.projectile->isFinished() && !pending.impactResolved) {
                resolveRangedImpact();
            }
        }

        const bool lungeDone = !attacker->isAttackLunging();
        const bool projectileDone = !pending.projectile || pending.projectile->isFinished();
        if (lungeDone && projectileDone && pending.impactResolved) {
            pending.phase = RangedAttackPhase::Finishing;
            activeRangedAttack.reset();
        }
    }
}

void AttackSystem::draw(sf::RenderTarget& target)
{
    if (activeRangedAttack && activeRangedAttack->projectile) {
        activeRangedAttack->projectile->draw(target);
    }
}

bool AttackSystem::isActive() const
{
    return activeRangedAttack.has_value();
}

bool AttackSystem::isAnimatingPawn(const Pawn* pawn) const
{
    if (!activeRangedAttack || !pawn) {
        return false;
    }
    return pawns->pawnDict[activeRangedAttack->attackerNum] == pawn
        && pawn->isAttackLunging();
}
