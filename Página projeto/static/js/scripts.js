let activeItem = null;

function toggleButtons(item) {
    if (activeItem && activeItem !== item) {
        activeItem.querySelector(".btn-container").style.display = "none";
        activeItem.querySelector(".loading").style.display = "none";
        activeItem.style.cursor = "pointer";
    } else if (item === activeItem) {
        return;
    }

    const loading = item.querySelector(".loading");
    const buttons = item.querySelector(".btn-container");
    loading.style.display = "block";
    buttons.style.display = "none";
    
    setTimeout(() => {
        loading.style.display = "none";
        buttons.style.display = "flex";
    }, 2000);
    
    item.style.cursor = "default";
    activeItem = item;
}

function enviarAcao(botao) {
    
}