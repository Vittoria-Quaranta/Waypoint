const monthYear = document.getElementById("month-year");
const calendarGrid = document.getElementById("calendar-grid");

const modal = document.getElementById("event-modal");
const selectedDateText = document.getElementById("selected-date");
const eventInput =
    document.getElementById("event-title-input");


let selectedEventId = null;

let currentDate = new Date();

let events = {};

let selectedDate = null;



function renderCalendar()
{

    calendarGrid.innerHTML = "";


    const year = currentDate.getFullYear();

    const month = currentDate.getMonth();


    const firstDay = new Date(year, month, 1);

    const daysInMonth =
        new Date(year, month + 1, 0).getDate();


    const monthNames = [
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December"
    ];


    monthYear.textContent =
        monthNames[month] + " " + year;



    for(let i=0; i<firstDay.getDay(); i++)
    {

        const blank = document.createElement("div");

        blank.className="day other-month";

        calendarGrid.appendChild(blank);

    }



    for(let day=1; day<=daysInMonth; day++)
    {

        const cell=document.createElement("div");

        cell.className="day";


        cell.innerHTML=`<strong>${day}</strong>`;


        const dateKey =
            `${year}-${month+1}-${day}`;



        if(events[dateKey])
        {

 events[dateKey].forEach(event => {

    const div=document.createElement("div");

    div.className="event";

    div.textContent = event.title;


    div.onclick = (e) =>
    {
        e.stopPropagation();

        openEditModal(event);
    };


    cell.appendChild(div);

});
        }



        cell.onclick=()=>openModal(dateKey);



        calendarGrid.appendChild(cell);

    }

}


async function loadEvents()
{
    const response =
        await fetch("/api/events");

    const data =
        await response.json();


    events = {};


    Object.values(data).forEach(event =>
    {

        if(!events[event.date])
        {
            events[event.date] = [];
        }


events[event.date].push(event);
    });


    renderCalendar();
}

function openModal(date)
{
    selectedEventId = null;

    document.getElementById("modal-title")
        .textContent = "Add Event";


    document.getElementById("delete-event")
        .style.display = "none";


    selectedDate = date;

    selectedDateText.textContent =
        "Date: " + date;


    eventInput.value = "";


    modal.classList.remove("hidden");
}



document.getElementById("save-event").onclick =
async () =>
{

    const title =
        eventInput.value;


    if(title.trim() === "")
        return;



    if(selectedEventId !== null)
    {

        await fetch(
            `/api/events/${selectedEventId}`,
            {
                method:"PUT",

                headers:
                {
                    "Content-Type":
                    "application/json"
                },


                body: JSON.stringify(
                {
                    date:selectedDate,
                    title:title
                })
            }
        );

    }

    else
    {

        await fetch(
            "/api/events",
            {
                method:"POST",

                headers:
                {
                    "Content-Type":
                    "application/json"
                },


                body: JSON.stringify(
                {
                    date:selectedDate,
                    title:title
                })
            }
        );

    }



    modal.classList.add("hidden");

    selectedEventId = null;

    loadEvents();

};



document.getElementById("close-modal").onclick=()=>{

    modal.classList.add("hidden");

};



document.getElementById("prev-month").onclick=()=>{

    currentDate.setMonth(
        currentDate.getMonth()-1
    );

    renderCalendar();

};

document.getElementById("delete-event")
.onclick = async () =>
{

    await fetch(
        `/api/events/${selectedEventId}`,
        {
            method:"DELETE"
        }
    );


    modal.classList.add("hidden");

    selectedEventId = null;

    loadEvents();

};

document.getElementById("next-month").onclick=()=>{

    currentDate.setMonth(
        currentDate.getMonth()+1
    );

    renderCalendar();

};

async function deleteEvent(id)
{

    await fetch(
        `/api/events/${id}`,
        {
            method:"DELETE"
        }
    );


    loadEvents();

}

function openEditModal(event)
{
    selectedEventId = event.id;

    document.getElementById("modal-title")
        .textContent = "Edit Event";


    selectedDateText.textContent =
        "Date: " + event.date;


    eventInput.value =
        event.title;


    document.getElementById("delete-event")
        .style.display = "inline-block";


    modal.classList.remove("hidden");
}

renderCalendar();
loadEvents();